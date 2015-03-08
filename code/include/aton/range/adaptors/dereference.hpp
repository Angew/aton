/*
Copyright Petr Kmoch 2015.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_RANGE_L_ADAPTORS_D_DEREFERENCE
#define ATON_GUARD_RANGE_L_ADAPTORS_D_DEREFERENCE

#include "aton/range/aton_/range_adaptor_wrapper.hpp"

#include "boost/range/adaptor/indirected.hpp"

#include <type_traits>


namespace aton {

namespace adaptors {

//! Type returned by #dereference adaptor.
/*!
%dereference_range is the return type of the #dereference adaptor.

\tparam SourceRange
	Type of the range on which #dereference is applied.
*/
template <class SourceRange>
using dereference_range = aton_::range_adaptor_wrapper_last<
	SourceRange,
	::boost::indirected_range<SourceRange>
>;


namespace aton_adaptors_ {

class dereference_adaptor
{
public:
	template <class SourceRange>
	dereference_range<typename ::std::remove_reference<SourceRange>::type>
	operator() (SourceRange &&source_range) const
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<
		dereference_range<typename ::std::remove_reference<SourceRange>::type>,
		SourceRange
	>::value)
	{
		return { ::std::forward<SourceRange>(source_range) };
	}
};


template <class SourceRange>
auto operator| (SourceRange &&range, const dereference_adaptor &adaptor)
ATON_NOEXCEPT_IF(noexcept(adaptor(::std::forward<SourceRange>(range))))
-> decltype(adaptor(::std::forward<SourceRange>(range)))
{
	return adaptor(::std::forward<SourceRange>(range));
}

}	//namespace aton_adaptors_


//! Adapt a range by dereferencing each element.
/*!
This adaptor wraps boost::adaptors::indirected and boost::adaptors::indirect for safe use with
rvalues. It can be called with zero or one arguments.

The zero-argument version is used for pipeline syntax (as an operand of the | operator).

The one-argument version is used for function call syntax. The parameter is the range whose
elements are to be dereferenced.
*/
const aton_adaptors_::dereference_adaptor dereference;

}	//namespace adaptors

}	//namespace aton

#endif	// ATON_GUARD_RANGE_L_ADAPTORS_D_DEREFERENCE
