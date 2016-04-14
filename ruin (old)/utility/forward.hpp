
#ifndef RUIN_UTILITY_FORWARD_HPP_INCLUDED
#define RUIN_UTILITY_FORWARD_HPP_INCLUDED

#include <type_traits>

namespace ruin
{
	template < class T >
	constexpr T&&
	forward(T& t) noexcept
	{
		return static_cast<T&&>(t);
	}
	template < class T >
	void
	forward(typename std::remove_reference<T>::type&&) = delete;
}

#endif // RUIN_UTILITY_FORWARD_HPP_INCLUDED
