cmake_minimum_required(VERSION 3.1.0)

project(module_php DESCRIPTION "module PhP pour le zia")
set(LIBRARY_NAME module_php)

set(SRCS
	src/php.cpp
	)

set(HDRS)

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
