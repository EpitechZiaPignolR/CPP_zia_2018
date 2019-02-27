/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ConfigManager.cpp
*/

#include <cstring>
#include "server/ConfigManager.hpp"

namespace zia::server {
	ConfigManager::ConfigManager(const std::string &&configFile):
	_config()
	{
		boost::property_tree::ptree root;
		try {
			boost::property_tree::read_json(configFile, root);
		}
		catch(std::exception & e) {
			std::cerr << "Error: Invalid config file" << std::endl;
			_config["modules"].v = dems::config::ConfigObject();
			_config["server"].v = dems::config::ConfigObject();
			auto &serverConfig = std::get<dems::config::ConfigObject>(_config["server"].v);
			serverConfig["ip"].v = "127.0.0.1";
			serverConfig["port"].v = 8080ll;
			return;
		}
		provideConfigObject(root, _config);
	}

	dems::config::ConfigValue ConfigManager::deduce_value(const std::string &s)
	{
		const char *p;
		char *end;
		dems::config::ConfigValue value;

		// check is long long
		p = s.c_str();
		value.v = std::strtoll(p, &end, 10);
		if (!p)
			return std::move(value);

		// check is double
		p = s.c_str();
		value.v = std::strtod(p, &end);
		if (!p)
			return std::move(value);

		if (s == "true") {
			value.v = true;
			return std::move(value);
		} else if (s == "false") {
			value.v = false;
			return std::move(value);
		} else if (s == "null") {
			value.v = nullptr;
			return std::move(value);
		} else {
			value.v = s;
			return std::move(value);
		}
	}

	void ConfigManager::provideConfigObject(const boost::property_tree::ptree &node,
		dems::config::ConfigObject &config)
	{
		for(auto &v: node) {
			if (v.second.data().empty()) {
				// it's an array or a node
				if (!v.second.empty() && v.second.begin()->first.empty()) {
					// it's an array
					config[v.first].v = dems::config::ConfigArray();
					auto &configArray = std::get<dems::config::ConfigArray>
					        (config[v.first].v);
					provideConfigArray(v.second, configArray);
				} else {
					// it's a node
					config[v.first].v = dems::config::ConfigObject();
					auto &configNode = std::get<dems::config::ConfigObject>
					        (config[v.first].v);
					if (!v.second.empty())
						provideConfigObject(v.second, configNode);
				}
			} else {
				auto value = deduce_value(v.second.data());
				config.emplace(v.first, std::move(value));
			}
		}
	}

	void ConfigManager::provideConfigArray(const boost::property_tree::ptree &node,
		dems::config::ConfigArray &config)
	{
		for(auto &v: node) {
			if (v.second.data().empty()) {
				// it's an array or a node
				if (!v.second.empty() && v.second.begin()->first.empty()) {
					// it's an array
					dems::config::ConfigValue value = {dems::config::ConfigArray()};
					config.push_back(std::move(value));
					auto &it = config.back();
					auto &configArray = std::get<dems::config::ConfigArray>(it.v);
					provideConfigArray(v.second, configArray);
				} else {
					// it's a node
					dems::config::ConfigValue value = {dems::config::ConfigObject()};
					config.push_back(std::move(value));
					auto &it = config.back();
					auto &configNode = std::get<dems::config::ConfigObject>(it.v);
					if (!v.second.empty())
						provideConfigObject(v.second, configNode);
				}
			} else {
				auto value = deduce_value(v.second.data());
				config.push_back(std::move(value));
			}
		}
	}

	dems::config::Config &&ConfigManager::getConfig()
	{
		return std::move(_config);
	}
}
