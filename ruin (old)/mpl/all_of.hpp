
#ifndef RUIN_MPL_ALL_OF_HPP_INCLUDED
#define RUIN_MPL_ALL_OF_HPP_INCLUDED

#include "ruin/mpl/list.hpp"
#include "ruin/mpl/logical_op.hpp"

namespace ruin { namespace mpl
{
	template < class, template < class > class >
	struct all_of;
	template < class... Types, template < class > class Pred >
	struct all_of<ruin::mpl::list<Types...>, Pred>
		: public ruin::mpl::and_<Pred<Types>...>
	{ };
}}

#endif // RUIN_MPL_ALL_OF_HPP_INCLUDED
