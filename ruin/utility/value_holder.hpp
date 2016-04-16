//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED
#define RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED

namespace ruin
{
	namespace detail
	{
		template<class T>
		struct value_holder_helper
		{
			typedef T value_type;
			typedef T& reference;
			typedef T const& const_reference;
			typedef T* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T holder_type;

			static constexpr holder_type const& hold(param_type p)
			{
				return p;
			}
			static constexpr reference ref(holder_type& r)
			{
				return r;
			}
			static constexpr const_reference ref(holder_type const& r)
			{
				return r;
			}
			static constexpr pointer ptr(holder_type& r)
			{
				return &r;
			}
			static constexpr const_pointer ptr(holder_type const& r)
			{
				return &r;
			}
		};
		template<class T>
		struct value_holder_helper<T const>
		{
			typedef T value_type;
			typedef T const& reference;
			typedef T const& const_reference;
			typedef T const* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T holder_type;

			static constexpr holder_type const& hold(param_type p)
			{
				return p;
			}
			static constexpr reference ref(holder_type& r)
			{
				return r;
			}
			static constexpr const_reference ref(holder_type const& r)
			{
				return r;
			}
			static constexpr pointer ptr(holder_type& r)
			{
				return &r;
			}
			static constexpr const_pointer ptr(holder_type const& r)
			{
				return &r;
			}
		};
		template<class T>
		struct value_holder_helper<T&>
		{
			typedef T value_type;
			typedef T& reference;
			typedef T const& const_reference;
			typedef T* pointer;
			typedef T const* const_pointer;
			typedef T& param_type;
			typedef T* holder_type;

			static constexpr holder_type hold(param_type p)
			{
				return &p;
			}
			static constexpr reference ref(holder_type r)
			{
				return *r;
			}
			static constexpr pointer ptr(holder_type r)
			{
				return r;
			}
		};
		template<class T>
		struct value_holder_helper<T const&>
		{
			typedef T value_type;
			typedef T const& reference;
			typedef T const& const_reference;
			typedef T const* pointer;
			typedef T const* const_pointer;
			typedef T const& param_type;
			typedef T const* holder_type;

			static constexpr holder_type hold(param_type p)
			{
				return &p;
			}
			static constexpr reference ref(holder_type r)
			{
				return *r;
			}
			static constexpr pointer ptr(holder_type r)
			{
				return r;
			}
		};
	}

	template<class T>
	class value_holder
	{
	public:
		typedef T type;
	private:
		typedef ruin::detail::value_holder_helper<type> helper_type;
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
		explicit constexpr value_holder(param_type p)
			: holder_(helper_type::hold(p))
		{ }
	public:
		explicit operator reference()
		{
			return helper_type::ref(holder_);
		}
		explicit constexpr operator const_reference() const
		{
			return helper_type::ref(holder_);
		}
		reference get()
		{
			return helper_type::ref(holder_);
		}
		constexpr const_reference get() const
		{
			return helper_type::ref(holder_);
		}
		pointer get_ptr()
		{
			return helper_type::ptr(holder_);
		}
		constexpr const_pointer get_ptr() const
		{
			return helper_type::ptr(holder_);
		}
		void swap(value_holder& y)
		{
			std::swap(holder_, y.holder_);
		}
	};
}

#endif // RUIN_UTILITY_VALUE_HOLDER_HPP_INCLUDED

