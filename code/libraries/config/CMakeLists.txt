# Copyright Petr Kmoch 2015.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

AddAtonLibrary(
	config INCLUDED
	LIBRARY_DIR config
	AGGREGATE config.hpp
	TOPDIR
		PUBLIC
			move_member_functions.hpp
			noexcept.hpp
)

if(BUILD_TESTING)
	add_subdirectory(test)
endif()
