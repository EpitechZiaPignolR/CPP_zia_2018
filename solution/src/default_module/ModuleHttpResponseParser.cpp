/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HttpResponseParser.cpp
*/

#include <cstdlib>
#include <cstring>
#include <variant>
#include "default_module/ModuleHttpResponseParser.hpp"

namespace zia::default_module {

	dems::CodeStatus HttpResponse(dems::Context &cont)
	{
		HttpResponseParser response(cont);

		auto ret = response.setResponse();
		return (ret);
	}

	HttpResponseParser::HttpResponseParser(dems::Context &cont) :
	_length(0), _left(0), _chunked(false), _cont(cont)
	{
		std::string data = "";

		if (!_cont.rawData.size())
			return;
		for (auto &x : _cont.rawData)
			data += x;
		_rest = data;
		zia::default_module::HttpResponseParser::mySplit(_heads, data, "\r\n");
	}

	HttpResponseParser::~HttpResponseParser()
	{ }

	dems::CodeStatus HttpResponseParser::setResponse()
	{
		size_t i;
		std::vector<std::string> line;

		if (!_heads.size())
			return dems::CodeStatus::DECLINED;
		if (zia::default_module::HttpResponseParser::checkFirstline(_heads[0], _cont) == 1)
			return (dems::CodeStatus::HTTP_ERROR);
		_heads.erase(_heads.begin());
		for (i = 0; i < _heads.size(); ++i)
		{
			if (_heads[i] == "")
				break;
			zia::default_module::HttpResponseParser::mySplit(line, _heads[i], "\r\n");
			if (line[0].compare("Content-Length"))
			{
				_length = std::atoi(line[1].c_str());
				_left = _length;
			}
			if (line[0].compare("Transfer-Encoding") && line[1].compare("chunked"))
				_chunked = true;
			_cont.request.headers->setHeader(line[0], line[1]);
			line.clear();
		}
		cleanRawData(i);
		if (_chunked == false)
			getStandardBody(_cont.rawData);
		return (dems::CodeStatus::OK);
	}

	void HttpResponseParser::cleanRawData(int i)
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
	}

	dems::CodeStatus HttpResponseParser::getChunk(std::vector<uint8_t> &data)
	{
		std::string body;
		size_t chunkSize;
		size_t total;

		for (auto & c : data)
			body += c;
		total = body.substr(0, body.find_first_of("\r\n")).length();
		if ((chunkSize = getChunkSize(body)) == 0)
		{
			data.clear();
			return (dems::CodeStatus::OK);
		}
		try
		{
			if (!body.substr(chunkSize - 2).compare("\r\n"))
				return (dems::CodeStatus::DECLINED);
		}
		catch (const std::exception& e)
		{
			return (dems::CodeStatus::DECLINED);
		}
		_rest += body.substr(body.find_first_of("\r\n") + 2, chunkSize);
		_cont.request.body = _rest;
		data.erase(data.begin(), data.begin() + total + 2 + chunkSize);
		return (dems::CodeStatus::OK);
	}

	ssize_t HttpResponseParser::getChunkSize(std::string &body)
	{
		size_t chunkSize;

		try
		{
			chunkSize = std::stoul(body.substr(0, body.find_first_of("\r\n")), nullptr, 16);
		}
		catch (const std::exception& e)
		{
			return (0);
		}
		if (chunkSize == 0)
			return (0);
		return (chunkSize);
	}

	dems::CodeStatus HttpResponseParser::getStandardBody(std::vector<uint8_t> &data)
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

	void HttpResponseParser::mySplit(std::vector<std::string> &dest, std::string &line, std::string const &delim)
	{
		std::string token = "";
		bool reg;

		for (size_t current = 0; current < line.length(); ++current)
		{
			reg = false;
			token += line[current];
			if (line[current] == delim[0])
			{
				reg = true;
				for (size_t j = 0; j < delim.length(); ++j)
					if ((current + j) < line.length() && line[current + j] != delim[j])
						reg = false;
				if (reg == true)
				{
					dest.push_back(token.substr(0, token.length() - 1));
					current += delim.length() - 1;
					token.clear();
				}
			}
		}
		if (token.length() > delim.length())
			dest.push_back(token);
	}

	int HttpResponseParser::checkFirstline(std::string &line, dems::Context &cont)
	{
		std::vector<std::string> Firstline;
		std::vector<std::string> versions = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1"};
		std::string splitChar = " ";

		zia::default_module::HttpResponseParser::mySplit(Firstline, line, splitChar);
		std::get<dems::header::Response>(cont.request.firstLine).httpVersion = "";
		for (auto & v : versions)
			if (Firstline[0].compare(v))
			{
				std::get<dems::header::Response>(cont.request.firstLine).httpVersion = Firstline[0];
				break ;
			}
		if (std::get<dems::header::Response>(cont.request.firstLine).httpVersion == "")
			return (1);
		std::get<dems::header::Response>(cont.request.firstLine).statusCode = Firstline[1];
		std::get<dems::header::Response>(cont.request.firstLine).message = Firstline[2];
		return (0);
	}
}