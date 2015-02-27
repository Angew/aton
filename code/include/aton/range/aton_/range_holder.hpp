/*
Copyright Petr Kmoch 2014-2015.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_RANGE_LL_RANGE_HOLDER
#define ATON_GUARD_RANGE_LL_RANGE_HOLDER

#include "aton/config.hpp"

#include <memory>
#include <type_traits>
#include <utility>


namespace aton {

namespace aton_ {

//! Wrapper which safely stores a reference to a range.
/*!
range_holder wraps a reference to a range. The range is accessed through the function held_range().

If constructed from an lvalue of #range_type, range_holder will simply store a pointer to the
range. If constructed from an rvalue, it moves that rvalue into its storage, promoting its lifetime
to that of the range_holder itself. This prevents the range, its iterators etc. from going out of
scope prematurely in contexts like using a range-based for loop to iterate over a range adaptor
pipeline.

\tparam Range
	Appropriately cv-qualified type of the range to be held.
*/
template <class Range>
class range_holder
{
public:
	using range_type = Range;
	using unqualified_range_type = typename ::std::remove_cv<Range>::type;

private:
	//! Holds the range if it's owned by the range_holder.
	typename ::std::aligned_storage<sizeof(unqualified_range_type)>::type value_;
	//! Points to the held range.
	/*!
	Can point to the range stored in #value_ (owning case), or to the range lvalue that was passed in
	the constructor (non-owning case).
	*/
	range_type *ptr_;

	bool owning_() const ATON_NOEXCEPT
	{
		return (
			static_cast<const volatile void *>(ptr_) ==
			static_cast<const volatile void *>(::std::addressof(value_))
		);
	}

	template <class SourceRange>
	range_type* acquire_ownership_(SourceRange &&sourceRange)
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<unqualified_range_type, SourceRange&&>::value)
	{
		return ::new (static_cast<void*>(::std::addressof(value_))) unqualified_range_type(
			::std::forward<SourceRange>(sourceRange)
		);
	}

	void release_ownership_()
	ATON_NOEXCEPT_IF(::std::is_nothrow_destructible<unqualified_range_type>::value)
	{
		held_range().~unqualified_range_type();
	}

public:
	~range_holder() ATON_NOEXCEPT_IF(::std::is_nothrow_destructible<unqualified_range_type>::value)
	{
		if (owning_()) {
			release_ownership_();
		}
	}

#if ATON_CFG_NO_GENERATED_MOVE_MEMBER_FUNCTIONS
	range_holder(const range_holder&) = default;

	range_holder(range_holder &&src)
	ATON_NOEXCEPT_IF(::std::is_nothrow_move_constructible<unqualified_range_type>::value) :
		ptr_(src.owning_() ? acquire_ownership_(std::move(src)) : src.ptr_)
	{}

	range_holder& operator= (const range_holder&) = default;

	range_holder& operator= (range_holder &&src)
	ATON_NOEXCEPT_IF(::std::is_nothrow_move_assignable<unqualified_range_type>::value)
	{
		if (BOOST_UNLIKELY(this == ::std::addressof(src))) {
			return *this;
		}
		if (owning_()) {
			if (src.owning_()) {
				held_range() = ::std::move(src.held_range());
			} else {
				release_ownership_();
				ptr_ = src.ptr_;
			}
		} else {
			if (src.owning_()) {
				ptr_ = acquire_ownership_(::std::move(src.held_range()));
			} else {
				ptr_ = src.ptr_;
			}
		}
		return *this;
	}

#endif

	//! Keep a pointer to \p range.
	explicit range_holder(range_type &range) ATON_NOEXCEPT :
		ptr_(::std::addressof(range))
	{}

	//! Move \p range into #value_.
	explicit range_holder(unqualified_range_type &&range)
	ATON_NOEXCEPT_IF(::std::is_nothrow_move_constructible<unqualified_range_type>::value) :
		ptr_(acquire_ownership_(::std::move(range)))
	{}

	//! Move \p range into #value_.
	template <
		class AtonSfinae =
			typename ::std::enable_if<!::std::is_same<range_type, unqualified_range_type>::value>::type
	>
	explicit range_holder(range_type &&range)
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<unqualified_range_type, range_type&&>::value) :
		ptr_(acquire_ownership_(::std::move(range)))
	{}
	
	const range_type& held_range() const ATON_NOEXCEPT
	{
		return *ptr_;
	}

	range_type& held_range() ATON_NOEXCEPT
	{
		return *ptr_;
	}
};

}	//namespace aton_

}	//namespace aton

#endif	// ATON_GUARD_RANGE_LL_RANGE_HOLDER
