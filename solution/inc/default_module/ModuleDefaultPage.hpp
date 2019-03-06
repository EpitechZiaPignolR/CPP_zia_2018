/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleDefaultPage.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleDefaultPage(dems::Context &context);

	inline std::string registerDefaultPageHooks(dems::StageManager &manager) {
		manager.request().hookToEnd(99999, "DefaultDefaultPage", ModuleDefaultPage);
		return "DefaultDefaultPage";
	}
}
