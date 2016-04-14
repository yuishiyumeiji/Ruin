
#ifndef RUIN_OPERATORS_EQUALITY_COMPARABLE_HPP_INCLUDED
#define RUIN_OPERATORS_EQUALITY_COMPARABLE_HPP_INCLUDED

namespace ruin
{
	template < class T, class U = T >
	class equality_comparable
	{
		friend constexpr bool
		operator==(U const& lhs, T const& rhs)
		{
			return rhs == lhs;
		}
		friend constexpr bool
		operator!=(T const& lhs, U const& rhs)
		{
			return !(lhs == rhs);
		}
		friend constexpr bool
		operator!=(U const& lhs, T const& rhs)
		{
			return rhs != lhs;
		}
	};
	template < class T >
	class equality_comparable<T, T>
	{
		friend constexpr bool
		operator!=(T const& lhs, T const& rhs)
		{
			return !(lhs == rhs);
		}
	};
}
	
#endif // RUIN_OPERATORS_EQUALITY_COMPARABLE_HPP_INCLUDED
