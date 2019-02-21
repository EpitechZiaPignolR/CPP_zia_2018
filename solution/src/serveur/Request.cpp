/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Request.cpp
*/

#include <iostream>
#include "server/Request.hpp"

namespace zia::server {
	Request::Request(dems::StageManager &stageManager, zia::server::SocketPtr &&socket):
	_socket(std::move(socket)),
	_stageManager(stageManager)
	{
		std::cout << "Request: connection" << std::endl;
		_stageManager.connection().firstHooks();
		_stageManager.connection().middleHooks();
		_stageManager.connection().endHooks();
	}

	void Request::handleRequest()
	{
		std::cout << "Request: handle request" << std::endl;
		_stageManager.request().firstHooks();
		_stageManager.request().middleHooks();
		_stageManager.request().endHooks();
	}

	Request::~Request()
	{
		std::cout << "Request: disconnect" << std::endl;
		_stageManager.disconnect().firstHooks();
		_stageManager.disconnect().middleHooks();
		_stageManager.disconnect().endHooks();

	}
}
