/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.cpp
*/

#include <functional>
#include <experimental/filesystem>
#include "dlloader/ModulesManager.hpp"
#include "dlloader/linux/DynamicLibrary.hpp"
#include "dlloader/windows/DynamicLibrary.hpp"

namespace zia::dlloader {
	void ModulesManager::loadModules(const std::string &directoryPath)
	{
		namespace fs = std::experimental::filesystem;

		for (const auto & entry: fs::directory_iterator(directoryPath))
			loadOneModule(entry.path());
	}

	void ModulesManager::loadOneModule(const std::string &filePath)
	{
#ifdef _WIN32
		windows_compatibility::DynamicLibrary
			loaded_library(std::forward(filePath));
#else
		unix_compatibility::DynamicLibrary
			loaded_library(std::forward(filePath));
#endif // _WIN32
		using RegisterFunction = std::function<void(dems::StageManager &)>;
		auto registerFunc = reinterpret_cast<RegisterFunction *>
			(loaded_library.loadSymbol("registerHooks"));
		(*registerFunc)(getStageManager());
	}
}
