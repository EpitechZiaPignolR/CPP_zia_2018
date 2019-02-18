/*
** EPITECH PROJECT, 2021
** Zia
** File description:
** main.cpp
*/

#include <iostream>
#include "zia.hpp"
#include "dlloader/ModulesManager.hpp"
#include "server/Server.hpp"

int main(int ac, char **av)
{
	try {
		(void)ac;
		(void)av;
		// Test
		zia::dlloader::ModulesManager modulesManager;

		modulesManager.loadOneModule("module/tester/libmodule_tester.so");

		zia::server::Server server("127.0.0.1", 8080);

		server.run([](zia::server::SocketPtr) -> void {
			std::cout << "lol" << std::endl;
		});
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Exit success 1" << std::endl;
	return EXIT_SUCCESS;
}
