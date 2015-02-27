/*
Copyright Petr Kmoch 2015.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_CONFIG_L_NOEXCEPT
#define ATON_GUARD_CONFIG_L_NOEXCEPT

#include "boost/config.hpp"


#if defined BOOST_NOEXCEPT_OR_NOTHROW
	#define ATON_NOEXCEPT BOOST_NOEXCEPT_OR_NOTHROW
#elif defined BOOST_NO_NOEXCEPT
	#define ATON_NOEXCEPT throw()
#else
	#define ATON_NOEXCEPT noexcept
#endif

#define ATON_NOEXCEPT_IF(...) BOOST_NOEXCEPT_IF(__VA_ARGS__)

#endif	// ATON_GUARD_CONFIG_L_NOEXCEPT
