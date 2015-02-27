/*
Copyright Petr Kmoch 2014.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_RANGE_L_ADAPTORS_D_TRANSFORM
#define ATON_GUARD_RANGE_L_ADAPTORS_D_TRANSFORM

#include "aton/range/aton_/range_adaptor_wrapper.hpp"

#include "boost/range/adaptor/transformed.hpp"

#include <type_traits>


namespace aton {

namespace adaptors {

//! Type returned by #transform adaptor.
/*!
%transform_range is the return type of the #transform adaptor.

\tparam SourceRange
	Type of the range on which #transform is applied.

\tparam Transformation
	Type of the transformation functor used by #transform.
*/
template <class SourceRange, class Transformation>
using transform_range = aton_::range_adaptor_wrapper_last<
	SourceRange,
	::boost::transformed_range<Transformation, SourceRange>
>;


namespace aton_adaptors_ {

template <class Transformation>
class transform_invoker
{
public:
	using transformation_type = Transformation;

private:
	transformation_type transformation_;

public:
	explicit transform_invoker(transformation_type &&transformation)
	ATON_NOEXCEPT_IF(::std::is_nothrow_move_constructible<transformation_type>::value) :
		transformation_(::std::move(transformation))
	{}

	template <class SourceRange>
	transform_range<typename ::std::remove_reference<SourceRange>::type, Transformation>
	operator() (SourceRange &&source_range)
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<
		transform_range<typename ::std::remove_reference<SourceRange>::type, Transformation>,
		SourceRange,
		Transformation&&
	>::value)
	{
		return { ::std::forward<SourceRange>(source_range), ::std::move(transformation_) };
	}
};


class transform_adaptor
{
public:
	template <class Transformation>
	transform_invoker<Transformation> operator() (Transformation transformation) const
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<
		transform_invoker<Transformation>,
		Transformation&&
	>::value)
	{
		return transform_invoker<Transformation>{ ::std::move(transformation) };
	}
	
	template <class SourceRange, class Transformation>
	transform_range<typename ::std::remove_reference<SourceRange>::type, Transformation>
	operator() (SourceRange &&source_range, Transformation transformation) const
	ATON_NOEXCEPT_IF(::std::is_nothrow_constructible<
		transform_range<typename ::std::remove_reference<SourceRange>::type, Transformation>,
		SourceRange,
		Transformation
	>::value)
	{
		return (*this)(::std::move(transformation))(::std::forward<SourceRange>(source_range));
	}
};


template <class SourceRange, class Transformation>
auto operator| (SourceRange &&range, transform_invoker<Transformation> &&adaptor)
ATON_NOEXCEPT_IF(noexcept(adaptor(::std::forward<SourceRange>(range))))
-> decltype(adaptor(::std::forward<SourceRange>(range)))
{
	return adaptor(::std::forward<SourceRange>(range));
}

}	//namespace aton_adaptors_


//! Adapt a range by applying a transformation to each element.
/*!
This adaptor wraps boost::adaptors::transformed and boost::adaptors::transform for safe use with
rvalues. It can be called with one or two arguments.

The one-argument version is used for pipeline syntax (as an operand of the | operator). The
parameter is the transformation to be applied. The returned adaptor stores the transformation by
value.

The two-argument version is used for function call syntax. The first parameter is the range to
transform and the second parameter is the transformation. The returned range stores the
transformation by value.
*/
const aton_adaptors_::transform_adaptor transform;

}	//namespace adaptors

}	//namespace aton

#endif	// ATON_GUARD_RANGE_L_ADAPTORS_D_TRANSFORM
