
#ifndef RUIN_MPL_EVAL_IF_HPP_INCLUDED
#define RUIN_MPL_EVAL_IF_HPP_INCLUDED

namespace ruin { namespace mpl
{
	template < bool Cond, class T1, class T2 >
	struct eval_if_c
	{
		typedef typename T1::type type;
	};
	template < class T1, class T2 >
	struct eval_if_c<false, T1, T2>
	{
		typedef typename T2::type type;
	};
	template < class Cond, class T1, class T2 >
	using eval_if = ruin::mpl::eval_if_c<Cond::value, T1, T2>;
}}

#endif // RUIN_MPL_EVAL_IF_HPP_INCLUDED

