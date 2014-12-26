/*
Copyright Petr Kmoch 2014.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef ATON_GUARD_INFRA_L_FUNCTION_CALLER
#define ATON_GUARD_INFRA_L_FUNCTION_CALLER

namespace aton {

template <class FunctionType, FunctionType &Function>
class function_caller;


template <class Result, class... Parameter, Result (&Function)(Parameter...)>
class function_caller<Result (Parameter...), Function>
{
public:
	Result operator() (Parameter&&... argument) const
	{
		return Function(std::forward<Parameter>(argument)...);
	}
};


#define ATON_FUNCTION_CALLER(Function) function_caller<decltype(Function), Function>

} // namespace aton

#endif // ATON_GUARD_INFRA_L_FUNCTION_CALLER
