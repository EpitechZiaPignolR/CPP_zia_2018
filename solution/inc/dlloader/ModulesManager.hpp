/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.hpp
*/

#pragma once

#include "api/AModulesManager.hpp"

typedef std::string (*RegisterFunction)(dems::StageManager &);

namespace zia::dlloader {

	class ModulesManager: public dems::AModulesManager {
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
