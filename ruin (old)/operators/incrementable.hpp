
#ifndef RUIN_OPERATORS_INCREMENTABLE_HPP_INCLUDED
#define RUIN_OPERATORS_INCREMENTABLE_HPP_INCLUDED

namespace ruin
{
	template < class T >
	class incrementable
	{
		friend constexpr T
		operator++(T& lhs, int)
		{
			auto temp = lhs;
			++lhs;
			return temp;
		}
	};
}

#endif // RUIN_OPERATORS_INCREMENTABLE_HPP_INCLUDED
