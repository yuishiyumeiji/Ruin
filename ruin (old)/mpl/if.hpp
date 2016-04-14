
#ifndef RUIN_MPL_IF_HPP_INCLUDED
#define RUIN_MPL_IF_HPP_INCLUDED

namespace ruin { namespace mpl
{
	template < bool Cond, class T1 = void, class T2 = void >
	struct if_c
	{
		typedef T1 type;
	};
	template < class T1, class T2 >
	struct if_c<false, T1, T2>
	{
		typedef T2 type;
	};
	template < class Cond, class T1 = void, class T2 = void >
	using if_ = ruin::mpl::if_c<Cond::value, T1, T2>;
}}

#endif // RUIN_MPL_LIST_HPP_INCLUDED

