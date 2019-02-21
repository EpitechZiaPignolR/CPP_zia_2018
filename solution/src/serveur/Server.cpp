/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Server.cpp
*/

#include <boost/bind.hpp>
#include <iostream>
#include "server/Server.hpp"

namespace zia::server {
	Server::Server(const std::string &&ip, unsigned short port):
		_io_service(),
		_signals(_io_service),
		_ip(boost::asio::ip::address_v4::from_string(ip)),
		_port(port),
		_endpoint(),
		_acceptor(_io_service),
		_threadPool(THREAD_NB)
	{
		_signals.add(SIGINT);
		_signals.add(SIGTERM);
#if defined(SIGQUIT)
		_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		_signals.async_wait(
			[this](boost::system::error_code, int) {
				stop();
				std::cout << "stop" << std::endl;
			});

		boost::asio::ip::tcp::resolver resolver(_io_service);
		_endpoint = *resolver.resolve({_ip, _port});

		_acceptor.open(_endpoint.protocol());
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		_acceptor.bind(_endpoint);
	}

	void Server::waitForConnection(const Callback &cb)
	{
		auto socket = SocketPtr(new boost::asio::ip::tcp::socket(_io_service));
		_acceptor.async_accept(*socket,
			boost::bind<void>([this, socket](const Callback &callback, const boost::system::error_code &err) {
				if (!_acceptor.is_open())
					return;
				if (err) {
					std::cout << "Error: when waiting for connection" << std::endl;
					throw;
				}
				boost::asio::post(_threadPool, boost::bind(callback, socket));
				waitForConnection(callback);
				}, cb, boost::asio::placeholders::error));
	}

	void Server::run(const Callback &cb)
	{
		_acceptor.listen();
		waitForConnection(cb);
		_io_service.run();
	}

	void Server::stop()
	{
		_acceptor.close();
		_io_service.stop();
	}
}
