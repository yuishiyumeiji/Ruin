
#ifndef RUIN_UTILITY_THROW_AND_RETURN_HPP_INCLUDED
#define RUIN_UTILITY_THROW_AND_RETURN_HPP_INCLUDED

#include <type_traits>

namespace ruin
{
	template < class ResultType >
	constexpr ResultType
	throw_and_return()
	{
		return throw, *static_cast<typename std::add_pointer<ResultType>::type>(nullptr);
	}
}

#endif // RUIN_UTILITY_THROW_AND_RETURN_HPP_INCLUDED

