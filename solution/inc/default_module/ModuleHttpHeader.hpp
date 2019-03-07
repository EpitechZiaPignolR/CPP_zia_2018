/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Html.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::html {
	class Header : private IHeader
	{
		std::map<std::string, std::string> _content;
		std::string _wholeHeader;
	public:
		Header();
		~Header();

		std::string &operator[](const std::string &headerName);
		std::string const &getHeader(const std::string &headerName);
		std::string getWholeHeaders();
		void setHeader(const std::string &headerName, const std::string &value);
	};
}