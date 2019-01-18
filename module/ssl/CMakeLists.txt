cmake_minimum_required(VERSION 3.1.0)

project(module_ssl DESCRIPTION "module ssl pour le zia")
set(LIBRARY_NAME module_ssl)

set(SRCS
	src/ssl.cpp
	)

set(HDRS)

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
