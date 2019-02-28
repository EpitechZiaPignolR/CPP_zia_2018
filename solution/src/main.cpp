/*
** EPITECH PROJECT, 2021
** Zia
** File description:
** main.cpp
*/

#include <iostream>
/*#include "dlloader/ModulesManager.hpp"
#include "server/Server.hpp"
#include "server/Request.hpp"
#include "server/ConfigManager.hpp"
*/
#include "server/ShellServer.hpp"

int main(int ac, char **av)
{
	if (ac != 2){
		std::cerr << "Error: Invalid number of argument" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		// get config
/*		zia::server::ConfigManager configManager(av[1]);
		auto config = std::move(configManager.getConfig());

		zia::dlloader::ModulesManager modulesManager(config);
		zia::server::Server server(config);

		auto handleRequest = zia::server::Callback(std::bind(
			[](dems::StageManager &stageManager, zia::server::SocketPtr socket)-> void{
				// lambda who handle a request
				zia::server::Request request(stageManager, std::move(socket));
				request.handleRequest();
			}, modulesManager.getStageManager(), std::placeholders::_1));

		server.run(handleRequest);*/
		zia::server::ShellServer shell(av[1]);
		shell.run();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Bye." << std::endl;
	return EXIT_SUCCESS;
}
