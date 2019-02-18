/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Server.hpp
*/

#pragma once

#include <boost/asio.hpp>

namespace zia::server {
	using SocketPtr = boost::shared_ptr<boost::asio::ip::tcp::socket>;
	using Callback = std::function<void(SocketPtr)>;

	class Server {
		static constexpr std::size_t THREAD_NB = 256;

		/* paramètres boost pour le serveur Tcp */
		boost::asio::io_service _io_service;
		boost::asio::signal_set _signals;

		boost::asio::ip::address_v4 _ip;
		unsigned short _port;

		boost::asio::ip::tcp::endpoint _endpoint;
		boost::asio::ip::tcp::acceptor _acceptor;
		boost::asio::ip::tcp::socket _socket;

		boost::asio::thread_pool _threadPool;

		void waitForConnection(const Callback &cb);
	public:
		/* Suppression des constructeur par copie */
		Server& operator=(const Server &) = delete;
		Server(const Server &) = delete;

		/* ctor et dtor */
		explicit Server(const std::string &&ip, unsigned short port);
		~Server() = default;

		void run(const Callback &cb);
		void stop();
	};
}
