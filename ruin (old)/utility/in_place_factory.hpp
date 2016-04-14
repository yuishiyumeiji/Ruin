
#ifndef RUIN_UTILITY_IN_PLACE_FACTORY_HPP_INCLUDED
#define RUIN_UTILITY_IN_PLACE_FACTORY_HPP_INCLUDED

#include <cstddef>
#include "ruin/tuple/tuple.hpp"
#include "ruin/tuple/tuple_get.hpp"
#include "ruin/utility/index_tuple.hpp"
#include "ruin/utility/forward.hpp"

namespace ruin
{
	template < class... Params >
	class in_place_factory
	{
	private:
		ruin::tuples::tuple<Params...> in_place_params_;

		template < class T, std::size_t... Indexes >
		void*
		apply_impl(void* address, ruin::index_tuple<Indexes...>) const
		{
			return ::new(address) T(ruin::tuples::tuple_get<Indexes>(in_place_params_)...);
		}
	public:
		explicit constexpr
		in_place_factory(Params... params)
			: in_place_params_(ruin::forward<Params>(params)...)
		{ }
		template < class T >
		void*
		apply(void* address) const
		{
			return apply_impl<T>(address, ruin::index_range<0, sizeof...(Params)>::make());
		}
		template < class T >
		void*
		apply(void* address, std::size_t n) const
		{
			if(n <= 0)
			{
				return nullptr;
			}
			char* next = static_cast<char*>(address = apply<T>(address));
			while(0<--n)
			{
				apply<T>(next = next + sizeof(T));
			}
			return address;
		}
	};

	template < class... Params >
	constexpr ruin::in_place_factory<Params&&...>
	in_place(Params&&... params)
	{
		return ruin::in_place_factory<Params&&...>(ruin::forward<Params&&>(params)...);
	}
}

#endif // RUIN_UTILITY_IN_PLACE_FACTORY_HPP_INCLUDED
