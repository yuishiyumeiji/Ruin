
#ifndef RUIN_VARIANT_STATIC_VISITOR_HPP_INCLUDED
#define RUIN_VARIANT_STATIC_VISITOR_HPP_INCLUDED

namespace ruin { namespace variants
{
	template < class ResultType = void >
	struct static_visitor
	{
		typedef ResultType result_type;
	};
}}

#endif // RUIN_VARIANT_STATIC_VISITOR_HPP_INCLUDED

