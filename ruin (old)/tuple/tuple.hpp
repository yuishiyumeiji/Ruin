
#ifndef RUIN_TUPLE_TUPLE_HPP_INCLUDED
#define RUIN_TUPLE_TUPLE_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include "ruin/mpl/at.hpp"
#include "ruin/mpl/list.hpp"
#include "ruin/tuple/tuple_get.hpp"
#include "ruin/tuple/tuple_size.hpp"
#include "ruin/tuple/traits.hpp"
#include "ruin/utility/declval.hpp"
#include "ruin/utility/index_tuple.hpp"
#include "ruin/utility/move.hpp"
#include "ruin/utility/forward.hpp"
#include "ruin/utility/piecewise_construct.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin
{
	namespace tuples
	{
		namespace detail
		{
			template < std::size_t I, class Head, bool = std::is_empty<Head>::value >
			struct head_base;
			template < std::size_t I, class Head >
			struct head_base<I, Head, false>
			{
			public:
				static constexpr Head&
				head(head_base& t) noexcept
				{
					return t.head_;
				}
				static constexpr Head const&
				head(head_base const& t) noexcept
				{
					return t.head_;
				}
			private:
				Head head_;
			public:
				head_base() = default;
				explicit constexpr
				head_base(Head const& h)
					: head_(h)
				{ }
				template < class... As >
				explicit constexpr
				head_base(As&&... as)
					: head_(ruin::forward<As>(as)...)
				{ }
			};
			template < std::size_t I, class Head >
			struct head_base<I, Head, true>
				: private Head
			{
			public:
				static constexpr Head&
				head(head_base& t) noexcept
				{
					return t;
				}
				static constexpr Head const&
				head(head_base const& t) noexcept
				{
					return t;
				}
			public:
				head_base() = default;
				explicit constexpr
				head_base(Head const& h)
					: Head(h)
				{ }
				template < class... As >
				explicit constexpr
				head_base(As&&... as)
					: Head(ruin::forward<As>(as)...)
				{ }
			};
			template < std::size_t, class... >
			class tuple_impl;
			template < std::size_t I >
			class tuple_impl<I>
			{
			protected:
				void
				swap(tuple_impl&) noexcept
				{ }
			public:
				tuple_impl() = default;
				constexpr
				tuple_impl(tuple_impl const&) = default;
				constexpr
				tuple_impl(tuple_impl&&) = default;
				template < class... As >
				explicit constexpr
				tuple_impl(As&&... as) noexcept
				{ }
				template < class... UTypes >
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I, UTypes...> const&) noexcept
				{ }
				template < class... UTypes >
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I, UTypes...>&&) noexcept
				{ }
			public:
				tuple_impl&
				operator=(tuple_impl const&) = default;
				tuple_impl&
				operator=(tuple_impl&&) = default;
				template < class... UTypes >
				tuple_impl&
				operator=(ruin::tuples::detail::tuple_impl<I, UTypes...> const&) noexcept
				{
					return *this;
				}
				template < class... UTypes >
				tuple_impl& operator=(ruin::tuples::detail::tuple_impl<I, UTypes...>&&) noexcept
				{
					return *this;
				}
			};
			template < std::size_t I, class Head, class... Tail >
			class tuple_impl<I, Head, Tail...>
				: public ruin::tuples::detail::tuple_impl<I + 1, Tail...>
				, private ruin::tuples::detail::head_base<I, Head>
			{
			public:
				typedef ruin::tuples::detail::tuple_impl<I + 1, Tail...> inherited;
				typedef ruin::tuples::detail::head_base<I, Head> base_type;
			public:
				static constexpr Head&
				head(tuple_impl& t) noexcept
				{
					return base_type::head(t);
				}
				static constexpr Head const&
				head(tuple_impl const& t) noexcept
				{
					return base_type::head(t);
				}
				static constexpr inherited&
				tail(tuple_impl& t) noexcept
				{
					return t;
				}
				static constexpr inherited const&
				tail(tuple_impl const& t) noexcept
				{
					return t;
				}
			protected:
				void
				swap(tuple_impl& y)
					noexcept(noexcept(ruin::swap(ruin::declval<Head&>(), head(y)))
					&& noexcept(ruin::declval<inherited&>().swap(tail(y)))
					)
				{
					ruin::swap(head(*this), head(y));
					inherited::swap(tail(y));
				}
			private:
				template < class Tuple, std::size_t... Indexes, class... As >
				explicit constexpr
				tuple_impl(Tuple&& t, ruin::index_tuple<Indexes...>, As&&... as)
					: inherited(ruin::piecewise_construct, ruin::forward<As>(as)...)
					, base_type(ruin::tuples::tuple_get<Indexes>(ruin::forward<Tuple>(t))...)
				{ }
			public:
				tuple_impl() = default;
				tuple_impl(tuple_impl const&) = default;
				explicit constexpr
				tuple_impl(Head const& h, Tail const&... t)
					: inherited(t...)
					, base_type(h)
				{ }
				template < class H, class... T >
				explicit constexpr
				tuple_impl(H&& h, T&&... t)
					: inherited(ruin::forward<T>(t)...)
					, base_type(ruin::forward<H>(h))
				{ }
				template < class Tuple, class... As >
				explicit constexpr
				tuple_impl(ruin::piecewise_construct_t, Tuple&& t, As&&... as)
					: tuple_impl(ruin::forward<Tuple>(t)
						, ruin::index_range<0, ruin::tuples::tuple_size<Tuple>::value>::make()
						, ruin::forward<As>(as)...
						)
				{ }
				template < class Tuple >
				constexpr
				tuple_impl(tuple_impl&& r)
					noexcept(std::is_nothrow_move_constructible<Head>::value
					&& std::is_nothrow_move_constructible<inherited>::value
					)
					: inherited(ruin::move(tail(r)))
					, base_type(ruin::forward<Head>(head(r)))
				{ }
				template < class... UTypes >
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I, UTypes...> const& r)
					: inherited(ruin::tuples::detail::tuple_impl<I, UTypes...>::tail(r))
					, base_type(ruin::tuples::detail::tuple_impl<I, UTypes...>::head(r))
				{ }
				template < class UHead, class... UTail >
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>&& r)
					: inherited(ruin::move(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>::tail(r)))
					, base_type(ruin::forward<UHead>(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>::head(r)))
				{ }
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I> const&)
					: inherited()
					, base_type()
				{ }
				constexpr
				tuple_impl(ruin::tuples::detail::tuple_impl<I>&&)
					: inherited()
					, base_type()
				{ }
			public:
				tuple_impl&
				operator=(tuple_impl const&) = default;
				tuple_impl&
				operator=(tuple_impl&& r)
					noexcept(std::is_nothrow_move_assignable<Head>::value
					&& std::is_nothrow_move_assignable<inherited>::value
					)
				{
					head(*this) = ruin::forward<Head>(head(r));
					tail(*this) = ruin::move(tail(r));
					return *this;
				}
				template < class... UTypes >
				tuple_impl&
				operator=(ruin::tuples::detail::tuple_impl<I, UTypes...> const& r)
				{
					head(*this) = ruin::tuples::detail::tuple_impl<I, UTypes...>::head(r);
					tail(*this) = ruin::tuples::detail::tuple_impl<I, UTypes...>::tail(r);
					return *this;
				}
				template < class UHead, class... UTail >
				tuple_impl&
				operator=(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>&& r)
				{
					head(*this) = ruin::forward<UHead>(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>::head(r));
					tail(*this) = ruin::move(ruin::tuples::detail::tuple_impl<I, UHead, UTail...>::tail(r));
					return *this;
				}
				tuple_impl&
				operator=(ruin::tuples::detail::tuple_impl<I> const&)
				{
					return *this;
				}
				tuple_impl&
				operator=(ruin::tuples::detail::tuple_impl<I>&&)
				{
					return *this;
				}
			};
		} // namespace detail

		template < class... Types >
		class tuple
			: public ruin::tuples::detail::tuple_impl<0, Types...>
		{
		public:
			typedef ruin::tuples::detail::tuple_impl<0, Types...> inherited;
		public:
			tuple() = default;
			explicit constexpr
			tuple(Types const&... elems)
				: inherited(elems...)
			{ }
			template < class... As >
			explicit constexpr
			tuple(As&&... as)
				: inherited(ruin::forward<As>(as)...)
			{ }
			tuple(tuple const&) = default;
			tuple(tuple&&) = default;
			template < class... UTypes >
			constexpr
			tuple(ruin::tuples::tuple<UTypes...> const& r)
				: inherited(static_cast<ruin::tuples::detail::tuple_impl<0, UTypes...> const&>(r))
			{ }
			template < class... UTypes >
			constexpr
			tuple(ruin::tuples::tuple<UTypes...>&& r)
				: inherited(static_cast<ruin::tuples::detail::tuple_impl<0, UTypes...>&&>(r))
			{ }
		public:
			tuple&
			operator=(tuple const& r)
			{
				static_cast<inherited&>(*this) = r;
				return *this;
			}
			tuple&
			operator=(tuple&& r)
				noexcept(std::is_nothrow_move_assignable<inherited>::value)
			{
				static_cast<inherited&>(*this) = ruin::move(r);
				return *this;
			}
			template < class... UTypes >
			tuple&
			operator=(ruin::tuples::tuple<UTypes...> const& r)
			{
				static_cast<inherited&>(*this) = r;
				return *this;
			}
			template < class... UTypes >
			tuple&
			operator=(ruin::tuples::tuple<UTypes...>&& r)
			{
				static_cast<inherited&>(*this) = ruin::move(r);
				return *this;
			}
		public:
			void
			swap(tuple& y)
				noexcept(noexcept(inherited::swap))
			{
				inherited::swap(y);
			}
		};
		template <>
		class tuple<>
		{
		public:
			void
			swap(tuple&) noexcept
			{ }
		};
		struct ignore_t
		{
			template < class T >
			ignore_t const&
			operator=(T const&) const
			{
				return *this;
			}
		};
		static constexpr ruin::tuples::ignore_t ignore{};

		template < class... Types >
		constexpr ruin::tuples::tuple<typename std::decay<Types>::type...>
		make_tuple(Types&&... args)
		{
			return ruin::tuples::tuple<typename std::decay<Types>::type...>(ruin::forward<Types>(args)...);
		}
		template < class... As >
		constexpr ruin::tuples::tuple<As&&...>
		forward_as_tuple(As&&... as) noexcept
		{
			return ruin::tuples::tuple<As&&...>(ruin::forward<As>(as)...);
		}
		template < class... Types >
		constexpr ruin::tuples::tuple<Types&...>
		make_tie(Types&... args) noexcept
		{
			return ruin::tuples::tuple<Types&...>(args...);
		}

		namespace detail
		{
			template < std::size_t I, class Head, class... Tail >
			constexpr Head&
			get_helper(ruin::tuples::detail::tuple_impl<I, Head, Tail...>& t) noexcept
			{
				return ruin::tuples::detail::tuple_impl<I, Head, Tail...>::head(t);
			}
			template < std::size_t I, class Head, class... Tail >
			constexpr Head const&
			get_helper(ruin::tuples::detail::tuple_impl<I, Head, Tail...> const& t) noexcept
			{
				return ruin::tuples::detail::tuple_impl<I, Head, Tail...>::head(t);
			}
		} // namespace detail

		template < class... Types >
		struct tuple_traits<ruin::tuples::tuple<Types...>>
		{
			static constexpr std::size_t size = sizeof...(Types);
			template < std::size_t I >
			struct element
				: public ruin::mpl::at_c<ruin::mpl::list<Types...>, I>
			{ };
			template < std::size_t I >
			static constexpr typename element<I>::type&
			get(ruin::tuples::tuple<Types...>& t) noexcept
			{
				return ruin::tuples::detail::get_helper<I>(t);
			}
			template < std::size_t I >
			static constexpr typename element<I>::type const&
			get(ruin::tuples::tuple<Types...> const& t) noexcept
			{
				return ruin::tuples::detail::get_helper<I>(t);
			}
			template < std::size_t I >
			static constexpr typename element<I>::type&&
			get(ruin::tuples::tuple<Types...>&& t) noexcept
			{
				return ruin::forward<typename element<I>::type>(ruin::tuples::detail::get_helper<I>(t));
			}
		};

		template < class... Types >
		void
		swap(ruin::tuples::tuple<Types...>& x, ruin::tuples::tuple<Types...>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace tuples

	using ruin::tuples::swap;
}

#endif // RUIN_TUPLE_TUPLE_HPP_INCLUDED

