/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Request.cpp
*/

#include <iostream>
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
