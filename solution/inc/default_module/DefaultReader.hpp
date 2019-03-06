/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DefaultReader.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "../api/Stage.hpp"

namespace zia::default_module {
	class DefaultReader {
		dems::Context &_context;

	public:
		DefaultReader(dems::Context &context);
		~DefaultReader() = default;

		dems::CodeStatus process();
	};

	dems::CodeStatus ModuleDefaultReader(dems::Context &context);

	inline std::string registerDefaultReaderHooks(dems::StageManager &manager) {
		manager.request().hookToEnd(100000, "DefaultReader", ModuleDefaultReader);
		manager.chunks().hookToFirst(100000, "DefaultReader", ModuleDefaultReader);
		return "DefaultReader";
	}
}
