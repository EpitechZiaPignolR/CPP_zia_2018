/*
** EPITECH PROJECT, 2021
** Zia
** File description:
** main.cpp
*/

#include <iostream>
#include "server/ShellServer.hpp"

int main(int ac, char **av)
{
	if (ac != 2){
		std::cerr << "Error: Invalid number of argument" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		zia::server::ShellServer shell(av[1]);
		shell.run();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Bye." << std::endl;
	return EXIT_SUCCESS;
}
