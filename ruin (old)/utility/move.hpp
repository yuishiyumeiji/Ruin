
#ifndef RUIN_UTILITY_MOVE_HPP_INCLUDED
#define RUIN_UTILITY_MOVE_HPP_INCLUDED

#include <type_traits>

namespace ruin
{
	template < class T >
	constexpr typename std::remove_reference<T>::type&&
	move(T&& t) noexcept
	{
		return static_cast<typename std::remove_reference<T>::type&&>(t);
	}
}

#endif // RUIN_UTILITY_MOVE_HPP_INCLUDED
