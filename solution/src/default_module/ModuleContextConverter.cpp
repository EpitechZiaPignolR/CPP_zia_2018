/*
** EPITECH PROJECT, 2019
** zia
** File description:
** code status
*/

#include "default_module/ModuleContextConverter.hpp"

namespace zia::default_module {
        dems::CodeStatus ModuleContextConverter(dems::Context &context)
        {
		std::string tmp;
		auto &response = context.response;
		try {
			auto &firstLine = std::get<dems::header::Response>(response.firstLine);
			tmp = firstLine.httpVersion + ' ' + firstLine.message + ' ' + firstLine.statusCode + "\r\n";
		} catch (std::exception &) {
			return dems::CodeStatus::HTTP_ERROR;
		}
		tmp += response.headers->getWholeHeaders() + "\r\n" + response.body;

		for (auto &it : tmp)
			context.rawData.push_back(it);
		return dems::CodeStatus::OK;
        }
}
