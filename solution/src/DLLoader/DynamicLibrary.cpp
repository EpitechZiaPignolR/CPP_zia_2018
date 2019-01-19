/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DynamicLibrary.cpp
*/

#include <iostream>
#include "DLLoader/DynamicLibrary.hpp"

namespace zia::dlloader {
	DynamicLibrary::DynamicLibrary(const std::string &library_name)
	{
		handle = dlopen(library_name.c_str(), RTLD_LAZY);
		if (handle == nullptr) {
			throw Exception(dlerror());
		}
	}

	DynamicLibrary::~DynamicLibrary()
	{
		if (dlclose(handle)) {
			std::cerr << dlerror() << std::endl;
		}
	}
}
