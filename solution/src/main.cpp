/*
** EPITECH PROJECT, 2021
** Zia
** File description:
** main.cpp
*/

#include "zia.hpp"
#include <iostream>

int main(int ac, char **av)
{
	try {
		(void)ac;
		(void)av;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Exit success 1" << std::endl;
	return EXIT_SUCCESS;
}
