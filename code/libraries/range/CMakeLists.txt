# Copyright Petr Kmoch 2014-2015.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

AddAtonLibrary(
	range INCLUDED
	LIBRARY_DIR range
	AGGREGATE range.hpp
	TOPDIR
		INTERNAL
			range_adaptor_wrapper.hpp
			range_holder.hpp
	SUBDIR adaptors
		PUBLIC
			dereference.hpp
			transform.hpp
)

if(BUILD_TESTING)
	add_subdirectory(test)
endif()
