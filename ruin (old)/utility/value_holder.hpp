
#ifndef RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED
#define RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED

#include "ruin/utility/address_of.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin
{
	namespace detail { namespace value_holder
	{
		template < class T >
		struct helper
		{
			typedef T value_type;
			typedef T& reference;
			typedef T const& const_reference;
			typedef T* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T holder_type;

			static constexpr holder_type const&
			hold (param_type p)
			{
				return p;
			}
			static constexpr reference
			ref (holder_type& r)
			{
				return r;
			}
			static constexpr const_reference
			ref (holder_type const& r)
			{
				return r;
			}
			static constexpr pointer
			ptr (holder_type& r)
			{
				return ruin::address_of(r);
			}
			static constexpr const_pointer
			ptr (holder_type const& r)
			{
				return ruin::address_of(r);
			}
		};
		template < class T >
		struct helper<T const>
		{
			typedef T value_type;
			typedef T const& reference;
			typedef T const& const_reference;
			typedef T const* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T holder_type;

			static constexpr holder_type const&
			hold (param_type p)
			{
				return p;
			}
			static constexpr reference
			ref (holder_type& r)
			{
				return r;
			}
			static constexpr const_reference
			ref (holder_type const& r)
			{
				return r;
			}
			static constexpr pointer
			ptr (holder_type& r)
			{
				return ruin::address_of(r);
			}
			static constexpr const_pointer
			ptr (holder_type const& r)
			{
				return ruin::address_of(r);
			}
		};
		template < class T >
		struct helper<T&>
		{
			typedef T value_type;
			typedef T& reference;
			typedef T const& const_reference;
			typedef T* pointer;
			typedef T const* const_pointer;
			typedef T& param_type;
			typedef T* holder_type;

			static constexpr holder_type
			hold (param_type p)
			{
				return ruin::address_of(p);
			}
			static constexpr reference
			ref (holder_type r)
			{
				return *r;
			}
			static constexpr pointer
			ptr (holder_type r)
			{
				return r;
			}
		};
		template < class T >
		struct helper<T const&>
		{
			typedef T value_type;
			typedef T const& reference;
			typedef T const& const_reference;
			typedef T const* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T const* holder_type;

			static constexpr holder_type
			hold (param_type p)
			{
				return ruin::address_of(p);
			}
			static constexpr reference
			ref (holder_type r)
			{
				return *r;
			}
			static constexpr pointer
			ptr (holder_type r)
			{
				return r;
			}
		};
	}} // namespace detail::value_holder

	template < class T >
	class value_holder
	{
	public:
		typedef T type;
	private:
		typedef ruin::detail::value_holder::helper<type> helper_type;
		typedef typename helper_type::holder_type holder_type;
	public:
		typedef typename helper_type::value_type value_type;
		typedef typename helper_type::reference reference;
		typedef typename helper_type::const_reference const_reference;
		typedef typename helper_type::pointer pointer;
		typedef typename helper_type::const_pointer const_pointer;
		typedef typename helper_type::param_type param_type;
	private:
		holder_type holder_;
	public:
		value_holder () = default;
		explicit constexpr
		value_holder (param_type p)
			: holder_(helper_type::hold(p))
		{ }
	public:
		explicit
		operator reference ()
		{
			return helper_type::ref(holder_);
		}
		explicit constexpr
		operator const_reference () const
		{
			return helper_type::ref(holder_);
		}
		reference
		get ()
		{
			return helper_type::ref(holder_);
		}
		constexpr const_reference
		get () const
		{
			return helper_type::ref(holder_);
		}
		pointer
		get_ptr ()
		{
			return helper_type::ptr(holder_);
		}
		constexpr const_pointer
		get_ptr () const
		{
			return helper_type::ptr(holder_);
		}
		void
		swap (value_holder& y)
			noexcept(noexcept(ruin::swap(value_holder::holder_, y.holder_)))
		{
			ruin::swap(holder_, y.holder_);
		}
	};

	template < class T >
	void
	swap (ruin::value_holder<T>& x, ruin::value_holder<T>& y)
		noexcept(noexcept(x.swap(y)))
	{
		x.swap(y);
	}
}

#endif // RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED

