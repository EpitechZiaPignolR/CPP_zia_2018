/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Header.cpp
*/

#include "Header/Header.hpp"

namespace zia::Header {
	Header::Header()
	{}

	Header::~Header()
	{}

	std::string			&Header::operator[](const std::string &headerName)
	{
		//boucle sur la map, compare la clé avec le headerName et retourne la valeur correspondante
		for ( const auto &Head : _content )
			if (Head.first.compare(headerName))
				return (&Head.second);
		return ("");
	}

	std::string const	&Header::getHeader(const std::string &headerName)
	{
		//boucle sur la map, compare la clé avec le headerName et retourne la valeur correspondante
		for ( const auto &Head : _content )
			if (Head.first.compare(headerName))
				return (&Head.second);
		return ("");
	}

	std::string			Header::getWholeHeaders()
	{
		_wholeHeader.clear();
		for (auto & x : _content)
		{
			_wholeHeader += x.first;
			_wholeHeader += ": ";
			_wholeHeader += x.second;
			_wholeHeader += "\r\n";
		}
		return (_wholeHeader);
	}

	void				Header::setHeader(const std::string &headerName,
										const std::string &value)
	{
		//check if headerName == trucs en particuler
		_content[headerName] = value;
	}
}