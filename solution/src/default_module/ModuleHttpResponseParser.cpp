/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HtmlResponseParser.cpp
*/

#include <cstdlib>
#include <cstring>
#include <boost/algorithm/string_regex.hpp>
#include "default_module/ModuleHttpResponseParser.hpp"

namespace zia::html {
	HtmlResponseParser::HtmlResponseParser(dems::Context &cont) :
	_length(0), _left(0), _chunked(false), _cont(cont)
	{
		std::string data;

		for (auto &x : _cont.rawData)
			data += x;
		_head._data = data;
		boost::algorithm::split_regex(_heads, data, regex("\r\n"));
		setHeader();
	}

	HtmlResponseParser::~HtmlResponseParser()
	{ }

	dems::CodeStatus HtmlResponseParser::setResponse()
	{
		size_t i;
		std::vector<std::string> line;

		if (checkFirstline(line) == 1)
			return (dems::CodeStatus::HTTP_ERROR);
		for (i = 0; _heads.size; ++i)
		{
			if (_heads[i].compare(""))
				break;
			boost::algorithm::split_regex(line, heads[i], regex(": "));
			if (line[0].compare("Content-Length"))
			{
				_length = std::atoi(line[1]);
				_left = length;
			}
			if (line[0].compare("Transfer-Encoding") && line[1].compare("chunked"))
				_chunked = true;
			_head.setHeader(line[0], line[1]);
			line.clear();
		}
		cleanRawData(i);
		getStandardBody(_cont.rawData);
		return (dems::CodeStatus::OK);
	}

	void HtmlResponseParser::cleanRawData(int i)
	{
		_rest.clear();
		_heads.erase(_heads.begin(), _heads.begin() + i);
		for (auto & line : _heads)
		{
			_rest += line;
			_rest += "\r\n";
		}
		_cont.rawData.clear();
		if (_length && _rest.size() > _length)
			std::copy(_rest.begin(), _rest.begin() + _length, std::back_inserter(_cont.rawData));
		else
			std::copy(_rest.begin(), _rest.end(), std::back_inserter(_cont.rawData));
		_cont.request.firstLine.method = _req.method;
		_cont.request.firstLine.path = _req.path;
		_cont.request.firstLine.httpVersion = _req.httpVersion;
		_cont.request.headers = _head;
	}

	dems::CodeStatus HtmlResponseParser::getChunk(std::vector<uint8_t> &data)
	{
		std::string body;

		for (auto & c : data)
			body += c;
		if (getChunkSize(data) == 0)
			return (dems::CodeStatus::OK);
		if (chunkSize != (body.length() - body.find_first_of("\r\n") - 2))
			return (dems::CodeStatus::DECLINED);
		_rest += body.substr(find_first_of("\r\n") + 2, chunkSize);
		_cont.request.body = _rest;
		data.erase(0, find_first_of("\r\n") + 2 + chunkSize);
		return (dems::CodeStatus::OK);
	}

	ssize_t HtmlResponseParser::getChunkSize(std::vector<uint8_t> &data)
	{
		size_t chunkSize;
		std::string body;

		for (auto & c : data)
			body += c;
		try
		{
			chunkSize = std::stoul(body.substr(0, body.find_first_of("\r\n")), nullptr, 16);
		}
		catch
		{
			return (0);
		}
		if (chunkSize == 0)
			return (0);
		return (chunkSize);
	}

	dems::CodeStatus HtmlResponseParser::getStandardBody(std::vector<uint8_t> &data)
	{
		std::string body;

		for (auto & c : data)
			body += c;
		if (_length != 0 && _length != body.length())
			return (dems::CodeStatus::DECLINED);
		_cont.request.body = body;
		data.clear();
		return (dems::CodeStatus::OK);
	}

	static int HtmlResponseParser::checkFirstline(std::string &line)
	{
		std::vector<std::string> Firstline;
		std::vector<std::string> versions = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1"}

		boost::split(line, data, boost::is_any_of(' '));
		_cont.request.firstLine.httpVersion = NULL;
		for (auto &version : versions)
			if (Firstline[].compare(version))
			{
				_cont.request.firstLine.httpVersion = Firstline[];
				break ;
			}
		if (_cont.request.firstLine.httpVersion == NULL)
			return (1);
		_cont.request.firstLine.statusCode = Firstline[1];
		_cont.request.firstLine.message = Firstline[1];
		return (0);
	}
}
