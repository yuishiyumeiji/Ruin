
#ifndef RUIN_UTILITY_DECAY_COPY_HPP_INCLUDED
#define RUIN_UTILITY_DECAY_COPY_HPP_INCLUDED

#include <type_traits>
#include "ruin/utility/forward.hpp"

namespace ruin
{
	template < class T >
	typename std::decay<T>::type
	decay_copy(T&& t) noexcept
	{
		return ruin::forward<T>(t);
	}
}

#endif // RUIN_UTILITY_DECAY_COPY_HPP_INCLUDED
