
#ifndef RUIN_UTILITY_MOVE_IF_NOEXCEPT_HPP_INCLUDED
#define RUIN_UTILITY_MOVE_IF_NOEXCEPT_HPP_INCLUDED

#include <type_traits>
#include "ruin/utility/move.hpp"

namespace ruin
{
	template < class T >
	constexpr typename std::conditional
		< !std::is_nothrow_move_constructible<T>::value
			&& std::is_copy_constructible<T>::value
		, T const&
		, T&&
		>::type
	move_if_noexcept(T& t) noexcept
	{
		return ruin::move(t);
	}
}

#endif // RUIN_UTILITY_MOVE_IF_NOEXCEPT_HPP_INCLUDED





