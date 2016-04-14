
#ifndef RUIN_UTILITY_ADDRESS_OF_HPP_INCLUDED
#define RUIN_UTILITY_ADDRESS_OF_HPP_INCLUDED

namespace ruin
{
	struct address_of_t
	{
		template < class T >
		constexpr T*
		operator()(T& t) const noexcept
		{
			return reinterpret_cast<T*>(&reinterpret_cast<char&>(t));
		}
		template < class T >
		constexpr T const*
		operator()(T const& t) const noexcept
		{
			return reinterpret_cast<T const*>(&reinterpret_cast<char const&>(t));
		}
		template < class T >
		constexpr T volatile*
		operator()(T volatile& t) const noexcept
		{
			return reinterpret_cast<T volatile*>(&reinterpret_cast<char volatile&>(t));
		}
		template < class T >
		constexpr T const volatile*
		operator()(T const volatile& t) const noexcept
		{
			return reinterpret_cast<T const volatile*>(&reinterpret_cast<char const volatile&>(t));
		}
	};
	static constexpr ruin::address_of_t address_of{};
}

#endif // RUIN_UTILITY_ADDRESS_OF_HPP_INCLUDED
