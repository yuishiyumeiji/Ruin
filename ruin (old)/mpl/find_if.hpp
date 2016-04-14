
#ifndef RUIN_MPL_FIND_IF_HPP_INCLUDED
#define RUIN_MPL_FIND_IF_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include "ruin/mpl/if.hpp"
#include "ruin/mpl/list.hpp"

namespace ruin { namespace mpl
{
	template < class, template < class > class, std::size_t = 0 >
	struct find_if;
	template < class Head, class... Tail, template < class > class Pred, std::size_t I >
	struct find_if<ruin::mpl::list<Head, Tail...>, Pred, I>
		: public ruin::mpl::if_
			< Pred<Head>, std::integral_constant<std::size_t, I>
			, ruin::mpl::find_if<ruin::mpl::list<Tail...>, Pred, I + 1>
			>::type
	{ };
	template < template < class > class Pred, std::size_t I >
	struct find_if<ruin::mpl::list<>, Pred, I>
		: public std::integral_constant<std::size_t, I>
	{ };
}}

#endif // RUIN_MPL_FIND_IF_HPP_INCLUDED

