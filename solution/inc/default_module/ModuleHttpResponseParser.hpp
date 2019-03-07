/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HtmlResponseParser.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "server/Headers.hpp"

namespace zia::html {
	
	class HtmlResponseParser
	{
		std::string _rest;
		size_t _length;
		size_t _left;
		bool _chunked;
		dems::Context &_cont;
		std::vector<std::string> _heads;
		dems::header::HTTPMessage _mess;
		dems::header::Response _res;
		zia::server::Headers _head;
		//enlever le header du rawdata
		void cleanRawData(int);

	public:
		HtmlResponseParser(dems::Context &cont);
		~HtmlResponseParser();

		dems::CodeStatus setResponse();
		//remettre tout en string et en fonction de chunked ou pas choisir la bonne fonction
		dems::CodeStatus getChunk(std::vector<uint8_t> &data);
		size_t getChunkSize(std::vector<uint8_t> &data);
		dems::CodeStatus getStandardBody(std::vector<uint8_t> &data);
		static int checkFirstline(std::string &);
	};
}
