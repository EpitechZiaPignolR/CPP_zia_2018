/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DynamicLibrary.hpp
*/

#pragma once
#include <dlfcn.h>
#include <string>

namespace zia::dlloader {
	class DynamicLibrary {
		/* Exception lié à Dynamic Library */
		class Exception: public std::exception {
			std::string _message;
		public:
			explicit Exception(const std::string &message)
				: _message("Error in DynamicLibrary: " + message)
			{}

			~Exception() override = default;

			const char *what() const noexcept override
			{ return _message.c_str(); }
		};

		/* Attricbuts de la classe */
		void *handle;

	public:
		/* Suppression des constructeur par copie */
		DynamicLibrary& operator=(const DynamicLibrary &) = delete;
		DynamicLibrary(const DynamicLibrary &) = delete;

		/* Méthodes de la classe */
		DynamicLibrary(const std::string &);
		~DynamicLibrary();

		template<typename T>
		T *loadSymbole(const std::string &symbolName)
		{
			void *symbol;

			symbol = dlsym(handle, symbolName.c_str());
			if (symbol == nullptr) {
				throw Exception(dlerror());
			}
			return reinterpret_cast<T *>(symbol);
		}
	};
}
