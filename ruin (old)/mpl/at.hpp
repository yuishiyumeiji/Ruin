
#ifndef RUIN_MPL_AT_HPP_INCLUDED
#define RUIN_MPL_AT_HPP_INCLUDED

#include <cstddef>
#include "ruin/mpl/list.hpp"

namespace ruin { namespace mpl
{
	template < class, std::size_t >
	struct at_c;
	template < class Head, class... Tail >
	struct at_c<ruin::mpl::list<Head, Tail...>, 0>
	{
		typedef Head type;
	};
	template < class Head, class... Tail, std::size_t I >
	struct at_c<ruin::mpl::list<Head, Tail...>, I>
		: public ruin::mpl::at_c<ruin::mpl::list<Tail...>, I - 1>
	{ };
	template < class T, class I >
	using at = ruin::mpl::at_c<T, I::value>;
}}

#endif // RUIN_MPL_AT_HPP_INCLUDED
