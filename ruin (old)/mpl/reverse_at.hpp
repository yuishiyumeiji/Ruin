
#ifndef RUIN_MPL_REVERSE_AT_HPP_INCLUDED
#define RUIN_MPL_REVERSE_AT_HPP_INCLUDED

#include <type_traits>
#include "ruin/mpl/list.hpp"

namespace ruin { namespace mpl
{
	template < class, class, std::size_t = 0 >
	struct reverse_at;
	template < class X, std::size_t I >
	struct reverse_at<ruin::mpl::list<>, X, I>
		: public std::integral_constant<std::size_t, I + 1>
	{ };
	template < class Head, class... Tail, std::size_t I >
	struct reverse_at<ruin::mpl::list<Head, Tail...>, Head, I>
		: public std::integral_constant<std::size_t, I>
	{ };
	template < class Head, class... Tail, class X, std::size_t I >
	struct reverse_at<ruin::mpl::list<Head, Tail...>, X, I>
		: public ruin::mpl::reverse_at<ruin::mpl::list<Tail...>, X, I + 1>
	{ };
}}

#endif // RUIN_MPL_REVERSE_AT_HPP_INCLUDED

