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
	Server::Server(dems::config::Config &config):
		_io_service(),
		_signals(_io_service),
		_ip(),
		_port(),
		_endpoint(),
		_acceptor(_io_service),
		_threadPool(THREAD_NB),
		_isRunning(false)
	{
		_signals.add(SIGINT);
		_signals.add(SIGTERM);
#if defined(SIGQUIT)
		_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		_signals.async_wait(
			[this](boost::system::error_code, int) {
				stop();
			});

		reloadConfig(config);
	}

	void Server::loadDefaultConfig(dems::config::Config &config)
	{
		_ip = boost::asio::ip::address_v4();
		_port = 8080;

		config["server"].v = dems::config::ConfigObject();
		auto &configServer = std::get<dems::config::ConfigObject>(config["server"].v);
		configServer["ip"].v = _ip.to_string();
		configServer["port"].v = static_cast<long long>(_port);
	}

	void Server::reloadConfig(dems::config::Config &config)
	{
		std::cout << "Setting up server..." << std::endl;
		if (!config.count("server"))
			loadDefaultConfig(config);
		else
			try {
				auto &configServer = std::get<dems::config::ConfigObject>(config["server"].v);
				if (!config.count("ip")) {
					_ip = boost::asio::ip::address_v4();
					configServer["ip"].v = _ip.to_string();
				} else
					try {
						auto &ip = std::get<std::string>(configServer["ip"].v);
						_ip = boost::asio::ip::address_v4::from_string(ip);
					} catch (const std::exception &) {
						_ip = boost::asio::ip::address_v4::from_string("");
					}
				if (!config.count("port")) {
					_port = 8080;
					configServer["port"].v = static_cast<long long>(_port);
				} else
					try {
						_port = std::get<long long>(configServer["port"].v);
					} catch (const std::exception &) {
						_port = 8080;
					}
			} catch (const std::exception &) {
				loadDefaultConfig(config);
			}
		std::cout << "Server address: " << _ip << ":" << _port << std::endl;

		boost::asio::ip::tcp::resolver resolver(_io_service);
		_endpoint = *resolver.resolve({_ip, _port});

		_acceptor.open(_endpoint.protocol());
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		_acceptor.bind(_endpoint);
		std::cout << "Server is set up" << std::endl;
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
		std::cout << "Running server..." << std::endl;
		_acceptor.listen();
		waitForConnection(cb);
		_isRunning = true;
		std::cout << "Server is running" << std::endl;
		_io_service.run();
	}

	void Server::stop()
	{
		std::cout << "Closing server..." << std::endl;
		_acceptor.close();
		_io_service.stop();
		_isRunning = false;
		std::cout << "Server is closed" << std::endl;
	}

	void Server::reload(dems::config::Config &config, const Callback &cb)
	{
		std::cout << "Reloading server..." << std::endl;
		auto oldState = _isRunning;
		if (oldState)
			stop();
		reloadConfig(config);
		if (oldState)
			run(cb);
		std::cout << "Server is reloaded" << std::endl;
	}
}
