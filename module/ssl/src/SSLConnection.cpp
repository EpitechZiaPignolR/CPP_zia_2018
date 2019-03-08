/*
** EPITECH PROJECT, 2021
** zia
** File description:
** SSLConnection.cpp
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "SSLConnection.hpp"

namespace zia::ssl_module {
	int my_ssl_verify_peer(int, X509_STORE_CTX*) {
		return 1;
	}

	bool SSLConnection::check_is_ssl()
	{
		char c;

		// check certificate
		try {
			if (!_context.config.count("modules")) {
				_context.config["is_ssl"].v = false;
				return false;
			}
			auto &confModule = std::get<dems::config::ConfigObject>(_context.config["modules"].v);
			if (!confModule.count("SSL")) {
				_context.config["is_ssl"].v = false;
				return false;
			}
			auto &confSSL = std::get<dems::config::ConfigObject>(confModule["SSL"].v);
			if (!confSSL.count("certificate") || !confSSL.count("certificate-key")) {
				_context.config["is_ssl"].v = false;
				return false;
			}
			_certificate = std::get<std::string>(confSSL["certificate"].v);
			_certificate_key = std::get<std::string>(confSSL["certificate-key"].v);
		} catch (std::exception &) {
			_context.config["is_ssl"].v = false;
			return false;
		}

		// check if the first character is uppercase letter
		recv(_context.socketFd, &c, 1, MSG_PEEK);
		bool is_ssl = !std::isupper(c);
		_context.config["is_ssl"].v = is_ssl;
		return is_ssl;
	}

	bool SSLConnection::init_SSL_CTX()
	{
		const SSL_METHOD *meth = DTLSv1_method();
		_ssl_ctx = SSL_CTX_new(meth);
		if (!_ssl_ctx) {
			std::cerr << "Error SSL: SSL_CTX_new" << std::endl;
			return true;
		}

		int err = SSL_CTX_set_cipher_list(_ssl_ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot set the cipher list" << std::endl;
			return true;
		}

		// set un callback bidon
		SSL_CTX_set_verify(_ssl_ctx, SSL_VERIFY_PEER, my_ssl_verify_peer);

		/* enable srtp */
		err = SSL_CTX_set_tlsext_use_srtp(_ssl_ctx, "SRTP_AES128_CM_SHA1_80");
		if(err != 0) {
			std::cerr << "Error SSL CTX: cannot setup srtp" << std::endl;
			return true;
		}
		err = SSL_CTX_use_certificate_file(_ssl_ctx, _certificate.c_str(), SSL_FILETYPE_PEM);
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot load certificate file: " << _certificate.c_str() << std::endl;
			return true;
		}
		err = SSL_CTX_use_PrivateKey_file(_ssl_ctx, _certificate_key.c_str(), SSL_FILETYPE_PEM);
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot load certificate-key file: " << _certificate_key.c_str() << std::endl;
			return true;
		}
		err = SSL_CTX_check_private_key(_ssl_ctx);
		if(err != 1) {
			std::cerr << "Error SSL CTX: checking the private key failed" << std::endl;
			return true;
		}
		return false;
	}

	bool SSLConnection::init_SSL()
	{
		_ssl = SSL_new(_ssl_ctx);
		if (!_ssl) {
			std::cerr << "Error: SSL new" << std::endl;
			return true;
		}

		/* bios */
		_in_bio = BIO_new(BIO_s_mem());
		if(_in_bio == NULL) {
			printf("Error: cannot allocate read bio.\n");
			return -2;
		}
		BIO_set_mem_eof_return(_in_bio, -1);

		_out_bio = BIO_new(BIO_s_mem());
		if(_out_bio == NULL) {
			printf("Error new BIO out: cannot allocate write bio.\n");
			return -3;
		}
		BIO_set_mem_eof_return(_out_bio, -1);

		SSL_set_bio(_ssl, _in_bio, _out_bio);


		SSL_set_accept_state(_ssl);

		SSL_set_fd(_ssl, _context.socketFd);
		int ret = SSL_connect(_ssl);
		if (!ret) {
			std::cerr << "Error: SSL Connect" << std::endl;
			return true;
		}
		if (ret < 0) {
			char msg[1024];
			int err = SSL_get_error(_ssl, ret);
			switch (err) {
			case SSL_ERROR_WANT_WRITE:
				std::cerr << "Error in init_SSL: SSL_ERROR_WANT_WRITE" << std::endl;
				return true;
			case SSL_ERROR_WANT_READ:
				std::cerr << "Error in init_SSL: SSL_ERROR_WANT_READ" << std::endl;
				return true;
			case SSL_ERROR_ZERO_RETURN:
				std::cerr << "Error in init_SSL: SSL_ERROR_ZERO_RETURN" << std::endl;
				return true;
			case SSL_ERROR_SYSCALL:
				std::cerr << "Error in init_SSL: SSL_ERROR_SYSCALL: " << ret << "-" << errno<< std::endl;
				return true;
			case SSL_ERROR_SSL:
				ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
				std::cerr << "Error in init_SSL: SSL_ERROR_SSL: " << msg << std::endl;
				return true;
			default:
				std::cerr << "Error in init_SSL: SSL Connect fatal error: " << std::endl;
				return true;
			}
		}
		return false;
	}

	SSLConnection::SSLConnection(dems::Context &context):
	_context(context),
	_certificate(),
	_certificate_key(),
	_ssl(nullptr),
	_ssl_ctx(nullptr),
	_in_bio(nullptr),
	_out_bio(nullptr)
	{
		if (_context.config.count("is_ssl"))
			_is_ssl = std::get<bool>(_context.config["is_ssl"].v);
		else
			_is_ssl = check_is_ssl();
		if (!_is_ssl)
			return;
		if (init_SSL_CTX() || init_SSL() || (!SSL_is_init_finished(_ssl) && SSL_do_handshake(_ssl))) {
			_is_ssl = false;
			_context.config["is_ssl"].v = false;
			return;
		}
	}

	SSLConnection::~SSLConnection()
	{
		if (_ssl) {
			SSL_shutdown(_ssl);
			SSL_free(_ssl);
		}
		if (_ssl_ctx) {
			SSL_CTX_free(_ssl_ctx);
		}
	}

	bool SSLConnection::is_ssl() const
	{
		return _is_ssl;
	}



	bool SSLConnection::read()
	{
		constexpr size_t BUFFER_SIZE = 256;
		int readByte;
		uint8_t buffer[BUFFER_SIZE];

		while ((readByte = SSL_read(_ssl, buffer, BUFFER_SIZE)) > 0) {
			if (readByte < 0) {
				break;
			}
			for (int i = 0; i < readByte; ++i) {
				_context.rawData.push_back(buffer[i]);
			}
		}
		if (readByte < 0) {
			int err = SSL_get_error(_ssl, readByte);
			switch (err) {
			case SSL_ERROR_WANT_WRITE:
				return false;
			case SSL_ERROR_WANT_READ:
				return false;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
			default:
				return true;
			}
		}
		return false;
	}

	bool SSLConnection::write()
	{
		std::cout << "----SSL WRITE----" << std::endl;
		auto size = static_cast<int>(_context.rawData.size() * sizeof(uint8_t));
		int err = SSL_write(_ssl, _context.rawData.data(), size);
		_context.rawData.clear();
		std::cout << "----SSL WRITE END----" << std::endl;
		if (err < 0) {
			err = SSL_get_error(_ssl, err);
			switch (err) {
			case SSL_ERROR_WANT_WRITE:
				return false;
			case SSL_ERROR_WANT_READ:
				return false;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
			default:
				return true;
			}
		}
		return false;
	}

}