
#ifndef RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED
#define RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED

namespace ruin
{
	template < class T >
	class decrementable
	{
		friend constexpr T
		operator--(T& lhs, int)
		{
			auto temp = lhs;
			--lhs;
			return temp;
		}
	};
}

#endif // RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED
