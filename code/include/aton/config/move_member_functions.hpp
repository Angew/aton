/*
Copyright Petr Kmoch 2015.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_CONFIG_L_MOVE_MEMBER_FUNCTIONS
#define ATON_GUARD_CONFIG_L_MOVE_MEMBER_FUNCTIONS

#include "boost/config.hpp"
#include "boost/detail/workaround.hpp"


#define ATON_CFG_NO_GENERATED_MOVE_MEMBER_FUNCTIONS \
	BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1800))

	#endif	// ATON_GUARD_CONFIG_L_MOVE_MEMBER_FUNCTIONS
