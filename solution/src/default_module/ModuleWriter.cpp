/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleWriter.cpp
*/

#include <cstdio>
#include "default_module/ModuleWriter.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleWriter(dems::Context &context)
	{
		std::string tmp;
		for (auto &it: context.rawData)
			tmp += it;
		dprintf(context.socketFd, "%s", tmp.c_str());
		return dems::CodeStatus::OK;
	}
}
