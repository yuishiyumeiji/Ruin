
#ifndef RUIN_UTILITY_INDEX_TUPLE_HPP_INCLUDED
#define RUIN_UTILITY_INDEX_TUPLE_HPP_INCLUDED

#include <cstddef>

namespace ruin
{
	template < std::size_t... >
	struct index_tuple
	{ };

	template
		< std::size_t First
		, std::size_t Last
		, std::size_t Difference = 1
		, class Range = ruin::index_tuple<>
		, bool Break = First >= Last
		>
	struct index_range
	{
		typedef Range type;

		static constexpr type
		make() noexcept
		{
			return {};
		}
	};
	template
		< std::size_t First
		, std::size_t Last
		, std::size_t Difference
		, std::size_t... Indexes
		>
	struct index_range<First, Last, Difference, ruin::index_tuple<Indexes...>, false>
		: public ruin::index_range<First + Difference, Last, Difference, ruin::index_tuple<Indexes..., First>>
	{ };

	template
		< std::size_t I
		, std::size_t Times
		, class Range = ruin::index_tuple<>
		, bool Break = Times == 0
		>
	struct index_n
	{
		typedef Range type;

		static constexpr type
		make() noexcept
		{
			return {};
		}
	};
	template < std::size_t I, std::size_t Times, std::size_t... Indexes >
	struct index_n<I, Times, ruin::index_tuple<Indexes...>, false>
		: public ruin::index_n<I, Times - 1, ruin::index_tuple<Indexes..., I>>
	{ };

	template < class Range = ruin::index_tuple<>, class ReversedRange = ruin::index_tuple<> >
	struct reverse_range
	{
		typedef ReversedRange type;

		static constexpr type
		make() noexcept
		{
			return {};
		}
	};
	template < std::size_t First, std::size_t... Indexes, std::size_t... Reversed >
	struct reverse_range<ruin::index_tuple<First, Indexes...>, ruin::index_tuple<Reversed...>>
		: public ruin::reverse_range<ruin::index_tuple<Indexes...>, ruin::index_tuple<First, Reversed...>>
	{ };

	template < class... Dummy >
	void
	no_op(Dummy&&...) noexcept
	{ }
}

#endif // RUIN_UTILITY_INDEX_TUPLE_HPP_INCLUDED
