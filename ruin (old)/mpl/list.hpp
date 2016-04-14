
#ifndef RUIN_MPL_LIST_HPP_INCLUDED
#define RUIN_MPL_LIST_HPP_INCLUDED

namespace ruin { namespace mpl
{
	template < class... >
	class list
	{ };

	using nil = ruin::mpl::list<>;
}}

#endif // RUIN_MPL_LIST_HPP_INCLUDED
