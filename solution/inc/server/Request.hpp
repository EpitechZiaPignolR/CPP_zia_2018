/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Request.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "server/Server.hpp"

namespace zia::server {
	class Request {
		zia::server::SocketPtr _socket;
		dems::StageManager &_stageManager;

	public:
		/* Suppression des constructeur par copie */
		Request& operator=(const Request &) = delete;
		Request(const Request &) = delete;

		/* ctor and dtor */
		explicit Request(dems::StageManager &stageManager, zia::server::SocketPtr &&);
		~Request();

		/* method */
		void handleRequest();
	};
}
