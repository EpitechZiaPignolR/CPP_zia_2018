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
		if (_modules.count(filePath))
			return;
#ifdef _WIN32
		_modules[filePath] = IDynamicLibraryPtr(new windows_compatibility::DynamicLibrary
			(std::forward<const std::string &&>(filePath)));
#else
		_modules[filePath] = IDynamicLibraryPtr(new unix_compatibility::DynamicLibrary
			(std::forward<const std::string &&>(filePath)));
#endif // _WIN32
		auto registerFunc = reinterpret_cast<RegisterFunction>
			(_modules[filePath]->loadSymbol("registerHooks"));
		(*registerFunc)(std::move(getStageManager()));
	}

	void ModulesManager::unloadModule(const std::string &)
	{}
}
