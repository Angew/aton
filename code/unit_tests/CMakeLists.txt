# Copyright Petr Kmoch 2015.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

get_filename_component(BoostTestSrcDir ${Boost_INCLUDE_DIRS}/libs/test/src ABSOLUTE)

aux_source_directory(${BoostTestSrcDir} BoostTestSources)

add_library(
	unit_tests STATIC
	${BoostTestSources}
)
target_compile_definitions(
	unit_tests
	PUBLIC
		BOOST_TEST_NO_LIB
		BOOST_TEST_ALTERNATIVE_INIT_API
)
