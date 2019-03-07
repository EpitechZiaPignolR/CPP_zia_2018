/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Request.cpp
*/

#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include "server/Request.hpp"

namespace zia::server {
	Request::Request(dems::config::Config &config, dems::StageManager &stageManager, zia::server::SocketPtr &&socket):
	_socket(std::move(socket)),
	_stageManager(stageManager),
	_context()
	{
		_context.config = config;
		_context.socketFd = _socket->native_handle();
		std::cout << "Request: connection" << std::endl;
		for (auto &func: _stageManager.connection().firstHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.connection().middleHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.connection().endHooks())
			func.second.callback(_context);
		if (_context.request.headers) {
			auto transferEncoding = (*_context.request.headers)["Transfer-Encoding"];
			if (transferEncoding == "chunked")
				handleChunks();
		}
	}

	void Request::handleChunks()
	{
		std::size_t timeOut = 0;
		std::size_t chunkSize = 0;
		do {
			for (auto &func: _stageManager.chunks().firstHooks())
				func.second.callback(_context);
			if (_context.rawData.empty()) {
				boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
				if (timeOut++ >= 3000)
					break;
				else
					continue;
			}
			timeOut = 0;
			// get chunksize;
			chunkSize = 0;
			for (auto &func: _stageManager.chunks().middleHooks())
				func.second.callback(_context);
			for (auto &func: _stageManager.chunks().endHooks())
				func.second.callback(_context);
		} while (chunkSize);
	}

	void Request::handleRequest()
	{
		std::cout << "Request: handle request" << std::endl;
		for (auto &func: _stageManager.request().firstHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.request().middleHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.request().endHooks())
			func.second.callback(_context);
	}

	Request::~Request()
	{
		std::cout << "Request: disconnect" << std::endl;
		for (auto &func: _stageManager.disconnect().firstHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.disconnect().middleHooks())
			func.second.callback(_context);
		for (auto &func: _stageManager.disconnect().endHooks())
			func.second.callback(_context);

	}
}
