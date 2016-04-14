
#ifndef RUIN_TUPLE_TUPLE_GET_HPP_INCLUDED
#define RUIN_TUPLE_TUPLE_GET_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include "ruin/tuple/traits.hpp"
#include "ruin/utility/forward.hpp"

namespace ruin { namespace tuples
{
	template < std::size_t I, class T, class Traits = ruin::tuples::tuple_traits<typename std::remove_reference<T>::type> >
	constexpr auto
	tuple_get(T&& t)
		noexcept(noexcept(Traits::template get<I>(ruin::forward<T>(t))))
		-> decltype(Traits::template get<I>(ruin::forward<T>(t)))
	{
		return Traits::template get<I>(ruin::forward<T>(t));
	}
}}

#endif // RUIN_TUPLE_TUPLE_GET_HPP_INCLUDED
