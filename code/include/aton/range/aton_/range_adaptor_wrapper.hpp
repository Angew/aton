/*
Copyright Petr Kmoch 2014-2015.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_RANGE_LL_RANGE_ADAPTOR_WRAPPER
#define ATON_GUARD_RANGE_LL_RANGE_ADAPTOR_WRAPPER

#include "aton/config.hpp"

#include "aton/range/aton_/range_holder.hpp"

#include <type_traits>
#include <utility>


namespace aton {

namespace aton_ {

//! Wrapper for a range adaptor capable of storing the adapted range internally.
/*!
range_adaptor_wrapper_first wraps an arbitrary range adaptor while using #range_holder to prevent
the underlying range from going out of scope prematurely.

Use this class template when the wrapped adaptor takes the underlying range as its \em first
constructor argument. If it takes it as the \em last argument, use range_adaptor_wrapper_last
instead.

\sa range_adaptor_wrapper_last
*/
template <class SourceRange, class WrappedAdaptor>
class range_adaptor_wrapper_first :
	private range_holder<SourceRange>,
	public WrappedAdaptor
{
	using holder_base = range_holder<SourceRange>;
	using adaptor_base = WrappedAdaptor;

public:
	using typename holder_base::range_type;
	using typename holder_base::unqualified_range_type;
	using adaptor_type = WrappedAdaptor;

	template <class... Arg>
	explicit range_adaptor_wrapper_first(range_type &range, Arg &&... arg)
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<adaptor_base, range_type&, Arg...>::value) :
		holder_base(range),
		adaptor_base(held_range(), ::std::forward<Arg>(arg)...)
	{}

	template <class... Arg>
	explicit range_adaptor_wrapper_first(unqualified_range_type &&range, Arg &&... arg)
	ATON_NOEXCEPT_IF(
		::std::is_nothrow_constructible<holder_base, unqualified_range_type&&>::value &&
		::std::is_nothrow_constructible<adaptor_base, range_type&, Arg...>::value
	) :
		holder_base(::std::move(range)),
		adaptor_base(held_range(), ::std::forward<Arg>(arg)...)
	{}

	template <
		class AtonSfinae = typename ::std::enable_if<!std::is_same<range_type, unqualified_range_type>::value>::type,
		class... Arg
	>
	explicit range_adaptor_wrapper_first(range_type &&range, Arg &&... arg)
	ATON_NOEXCEPT_IF(
		::std::is_nothrow_constructible<holder_base, range_type&&>::value &&
		::std::is_nothrow_constructible<adaptor_base, range_type&, Arg...>::value
	) :
		holder_base(::std::move(range)),
		adaptor_base(held_range(), ::std::forward<Arg>(arg)...)
	{}
};



//! Wrapper for a range adaptor capable of storing the adapted range internally.
/*!
range_adaptor_wrapper_last wraps an arbitrary range adaptor while using #range_holder to prevent
the underlying range from going out of scope prematurely.

Use this class template when the wrapped adaptor takes the underlying range as its \em last
constructor argument. If it takes it as the \em first argument, use range_adaptor_wrapper_first
instead.

\sa range_adaptor_wrapper_first
*/
template <class SourceRange, class WrappedAdaptor>
class range_adaptor_wrapper_last :
	private range_holder<SourceRange>,
	public WrappedAdaptor
{
	using holder_base = range_holder<SourceRange>;
	using adaptor_base = WrappedAdaptor;

public:
	using typename holder_base::range_type;
	using typename holder_base::unqualified_range_type;
	using adaptor_type = WrappedAdaptor;

	template <class... Arg>
	range_adaptor_wrapper_last(range_type &range, Arg &&... arg)
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<adaptor_base, Arg..., range_type&>::value) :
		holder_base(range),
		adaptor_base(::std::forward<Arg>(arg)..., held_range())
	{}

	template <class... Arg>
	range_adaptor_wrapper_last(unqualified_range_type &&range, Arg &&... arg)
	ATON_NOEXCEPT_IF(
		::std::is_nothrow_constructible<holder_base, unqualified_range_type&&>::value &&
		::std::is_nothrow_constructible<adaptor_base, Arg..., range_type&>::value
	) :
		holder_base(::std::move(range)),
		adaptor_base(::std::forward<Arg>(arg)..., held_range())
	{}

	template <
		class AtonSfinae = typename ::std::enable_if<!std::is_same<range_type, unqualified_range_type>::value>::type,
		class... Arg
	>
	range_adaptor_wrapper_last(range_type &&range, Arg &&... arg)
	ATON_NOEXCEPT_IF(
		::std::is_nothrow_constructible<holder_base, range_type&&>::value &&
		::std::is_nothrow_constructible<adaptor_base, Arg..., range_type&>::value
	) :
		holder_base(::std::move(range)),
		adaptor_base(::std::forward<Arg>(arg)..., held_range())
	{}
};

}	//namespace aton_

}	//namespace aton

#endif	// ATON_GUARD_RANGE_LL_RANGE_ADAPTOR_WRAPPER
