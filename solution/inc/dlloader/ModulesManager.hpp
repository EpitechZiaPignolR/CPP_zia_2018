/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.hpp
*/

#pragma once

#include "api/AModulesManager.hpp"

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

		void loadModules(const std::string &directoryPath);
		void loadOneModule(const std::string &filePath);
	};
}
