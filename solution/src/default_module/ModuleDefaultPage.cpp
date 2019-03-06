/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleDefaultPage.cpp
*/

#include "default_module/ModuleDefaultPage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleDefaultPage(dems::Context &context)
	{
		try {
			auto &firstLine = std::get<dems::header::Response>(context.response.firstLine);
			if (!firstLine.statusCode.empty())
				return dems::CodeStatus::OK;

		} catch (std::exception &e) {}
		context.response = {
			dems::header::Response{"HTTP/1.1", "404", "Not Found"},
			nullptr,
			"Page Not Found LoL"
		};
		return dems::CodeStatus::OK;
	}
}
