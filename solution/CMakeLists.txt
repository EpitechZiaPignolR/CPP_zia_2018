cmake_minimum_required(VERSION 3.1.0)

project("zia_solution" DESCRIPTION "Solution Zia")
set(BINARY_NAME zia)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wold-style-cast -Wcast-align -Wunused \
	-Woverloaded-virtual -Wpedantic -Wsign-conversion -Wlogical-op -Wuseless-cast -Weffc++")

set(INCLUDE inc)

set(SRCS
	src/main.cpp
	src/dlloader/linux/DynamicLibrary.cpp
	src/dlloader/windows/DynamicLibrary.cpp
	#src/serveur/Server.cpp
	src/dlloader/ModulesManager.cpp)

set(HDRS
	inc/zia.hpp
	inc/api/AModulesManager.hpp
	inc/api/Heading.hpp
	inc/api/Stage.hpp
	inc/dlloader/linux/DynamicLibrary.hpp
	inc/dlloader/windows/DynamicLibrary.hpp
	inc/dlloader/IDynamicLibrary.hpp
	inc/dlloader/ModulesManager.hpp
	#inc/server/Server.hpp
	#inc/server/IServer.hpp
	)

set(LIBS
	"-ldl -lstdc++fs")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

message(STATUS ${SRCS_CLIENT})
message(STATUS ${SRCS_SERVER})

add_executable(${BINARY_NAME} ${SRCS} ${HDRS})
target_include_directories(${BINARY_NAME} PRIVATE ${INCLUDE})
target_link_libraries(${BINARY_NAME} ${CONAN_LIBS} ${LIBS})
