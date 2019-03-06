/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DefaultReader.cpp
*/

#include "default_module/DefaultReader.hpp"

#ifdef __linux__
	#include <sys/types.h>
	#include <unistd.h>
	#include <fcntl.h>
#endif

#include <iostream>

namespace zia::default_module {
	DefaultReader::DefaultReader(dems::Context &context):
	_context(context)
	{}


	dems::CodeStatus DefaultReader::process()
	{
		constexpr size_t BUFFER_SIZE = 256;
		unsigned char buffer[BUFFER_SIZE + 1];
		ssize_t readByte;
		int flags;

		if (!_context.rawData.empty())
			return dems::CodeStatus::OK;
#ifdef __linux__
		// récupère les flags du fd
		flags = fcntl(_context.socketFd, F_GETFL, 0);
		// rend le read non-bloquant
		fcntl(_context.socketFd, F_SETFL, flags | O_NONBLOCK);
		// lis la socket
		while ((readByte = read(_context.socketFd, buffer, BUFFER_SIZE))) {
			if (readByte == -1 && errno == EAGAIN) {
				break;
			} else if (readByte == -1) {
				fcntl(_context.socketFd, F_SETFL, flags);
				return dems::CodeStatus::HTTP_ERROR;
			}
			for (int i = 0; i < readByte; ++i) {
				_context.rawData.push_back(buffer[i]);
			}
		}
		// remet les anciens flags
		fcntl(_context.socketFd, F_SETFL, flags);
#else
		// active le socket en non-bloquant
		flags = 1;
		ioctlsocket(socket, FIONBIO, &flags);
		readByte = recv(_context.socketFd, buffer, BUFFER_SIZE, );
		while ((readByte = read(_context.socketFd, buffer, BUFFER_SIZE))) {
			if (readByte < 0 && WSAGetLastError() != WSAEWOULDBLOCK)
				break;
			else if (readByte < 0) {
				flags = 0;
				ioctlsocket(socket, FIONBIO, &flags);
				return dems::CodeStatus::HTTP_ERROR;
			}
			for (int i = 0; i < readByte; ++i)
				_context.rawData.push_back(buffer[i]);
		}
		// remet en bloquant
		flags = 0;
		ioctlsocket(socket, FIONBIO, &flags);
#endif
		std::string tmp;
		for (auto &it: _context.rawData)
			tmp += it;
		return dems::CodeStatus::OK;
	}

	dems::CodeStatus ModuleDefaultReader(dems::Context &context)
	{
		DefaultReader defaultReader(context);
		return defaultReader.process();
	}
}
