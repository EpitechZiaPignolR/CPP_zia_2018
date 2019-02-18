/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.hpp
*/

#pragma once

#include <unordered_map>
#include "api/AModulesManager.hpp"
#include "dlloader/IDynamicLibrary.hpp"

typedef std::string (*RegisterFunction)(dems::StageManager &&);

namespace zia::dlloader {

	class ModulesManager: public dems::AModulesManager {
		using IDynamicLibraryPtr = std::unique_ptr<IDynamicLibrary>;
		std::unordered_map<std::string, IDynamicLibraryPtr> _modules;

	public:
		/* Suppression des constructeur par copie */
		ModulesManager& operator=(const ModulesManager &) = delete;
		ModulesManager(const ModulesManager &) = delete;

		/* Destructor */
		~ModulesManager() override = default;

		/* Constructor */
		ModulesManager() = default;

		void loadModules(const std::string &directoryPath) override;
		void loadOneModule(const std::string &filePath) override;

		void unloadModule(const std::string &moduleName) override;
	};
}
