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
		BIO *_in_bio;
		BIO *_out_bio;

		bool _is_ssl;

		bool check_is_ssl();
		bool init_SSL_CTX();
		bool init_SSL();
	public:
		explicit SSLConnection(dems::Context &);
		~SSLConnection();

		bool read();
		bool write();

		bool is_ssl() const;
	};
}
