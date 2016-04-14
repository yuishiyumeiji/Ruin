
#ifndef RUIN_MPL_IDENTITY_HPP_INCLUDED
#define RUIN_MPL_IDENTITY_HPP_INCLUDED

namespace ruin { namespace mpl
{
	template < class T >
	struct identity
	{
		typedef T type;
	};
}}

#endif // RUIN_MPL_IDENTITY_HPP_INCLUDED

