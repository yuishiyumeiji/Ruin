
#ifndef RUIN_MPL_MAP_HPP_INCLUDED
#define RUIN_MPL_MAP_HPP_INCLUDED

#include "ruin/mpl/list.hpp"

namespace ruin { namespace mpl
{
	template < template < class... > class, class >
	struct map;
	template < template < class... > class F, class... Types >
	struct map<F, ruin::mpl::list<Types...>>
	{
		typedef ruin::mpl::list<typename F<Types>::type...> type;
	};
}}

#endif // RUIN_MPL_MAP_HPP_INCLUDED
