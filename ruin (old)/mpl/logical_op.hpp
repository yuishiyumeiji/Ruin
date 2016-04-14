
#ifndef RUIN_MPL_LOGICAL_OP_HPP_INCLUDED
#define RUIN_MPL_LOGICAL_OP_HPP_INCLUDED

#include <type_traits>
#include "ruin/mpl/if.hpp"

namespace ruin { namespace mpl
{
	template < bool L, bool R >
	struct and_c
		: public std::integral_constant<bool, L && R>
	{ };
	template < bool L, bool R >
	struct or_c
		: public std::integral_constant<bool, L || R>
	{ };
	template < bool L >
	struct not_c
		: public std::integral_constant<bool, !L>
	{ };

	template < class... >
	struct and_;
	template <>
	struct and_<>
		: public std::true_type
	{ };
	template < class Head >
	struct and_<Head>
		: public Head
	{ };
	template < class Head, class... Tail >
	struct and_<Head, Tail...>
		: public ruin::mpl::if_<Head, ruin::mpl::and_<Tail...>, std::false_type>::type
	{ };

	template < class... >
	struct or_;
	template <>
	struct or_<>
		: public std::false_type
	{ };
	template < class Head >
	struct or_<Head>
		: public Head
	{ };
	template < class Head, class... Tail >
	struct or_<Head, Tail...>
		: public ruin::mpl::if_<Head, std::true_type, ruin::mpl::or_<Tail...>>::type
	{ };

	template < class Head >
	struct not_
		: public ruin::mpl::not_c<Head::value>
	{ };
}}

#endif // RUIN_MPL_LOGICAL_OP_HPP_INCLUDED

