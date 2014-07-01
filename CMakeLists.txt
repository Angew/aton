# Copyright Petr Kmoch 2014.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
project(aton)

set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
	set(DefInstallCmakeDir CMake)
else()
	set(DefInstallCmakeDir lib/CMake/aton)
endif()
set(INSTALL_CMAKE_DIR ${DefInstallCmakeDir} CACHE PATH "Installation directory for CMake files")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()
option(BUILD_TESTING "Generate tests" ON)

add_subdirectory(code)
