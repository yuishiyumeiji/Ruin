
#ifndef RUIN_MPL_ANY_OF_HPP_INCLUDED
#define RUIN_MPL_ANY_OF_HPP_INCLUDED

#include "ruin/mpl/list.hpp"
#include "ruin/mpl/logical_op.hpp"

namespace ruin { namespace mpl
{
	template < class, template < class > class >
	struct any_of;
	template < class... Types, template < class > class Pred >
	struct any_of<ruin::mpl::list<Types...>, Pred>
		: public ruin::mpl::or_<Pred<Types>...>
	{ };
}}

#endif // RUIN_MPL_ANY_OF_HPP_INCLUDED
