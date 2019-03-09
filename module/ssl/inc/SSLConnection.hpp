/*
** EPITECH PROJECT, 2021
** zia
** File description:
** SSLConnection.hpp
*/

#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "api/Stage.hpp"

namespace zia::ssl_module {
	class SSLConnection {
		dems::Context &_context;
		std::string _certificate;
		std::string _certificate_key;

		SSL *_ssl;
		SSL_CTX *_ssl_ctx;

		bool _is_ssl;

		bool initCertificate();
		bool checkIsSSL();
		bool initSSL_CTX();
		bool initSSL();
	public:
		explicit SSLConnection(dems::Context &);
		~SSLConnection();

		bool read();
		bool write();

		bool isSSL() const;
	};
}
