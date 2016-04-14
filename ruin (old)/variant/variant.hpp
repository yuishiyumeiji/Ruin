
#ifndef RUIN_VARIANT_VARIANT_HPP_INCLUDED
#define RUIN_VARIANT_VARIANT_HPP_INCLUDED

#include <type_traits>
#include "ruin/mpl/all_of.hpp"
#include "ruin/mpl/at.hpp"
#include "ruin/mpl/eval_if.hpp"
#include "ruin/mpl/find_if.hpp"
#include "ruin/mpl/identity.hpp"
#include "ruin/mpl/if.hpp"
#include "ruin/mpl/list.hpp"
#include "ruin/utility/address_of.hpp"
#include "ruin/utility/forward.hpp"
#include "ruin/utility/move.hpp"
#include "ruin/variant/static_visitor.hpp"
#include "ruin/variant/traits.hpp"

namespace ruin
{
	namespace variants
	{
		struct blank
		{ };

		namespace detail
		{
			template < class, class, int = 0 >
			struct find_index;
			template < class T, int I >
			struct find_index<ruin::mpl::list<>, T, I>
				: public std::integral_constant<int, I>
			{ };
			template < class Head, class... Tail, class U, int I >
			struct find_index<ruin::mpl::list<Head, Tail...>, U, I>
				: public ruin::mpl::if_
					< std::is_same<typename std::decay<U>::type, Head>
					, std::integral_constant<int, I>
					, ruin::variants::detail::find_index<ruin::mpl::list<Tail...>, U, I + 1>
					>::type
			{ };

			template < class... >
			union trivial_storage;
			template <>
			union trivial_storage<>
			{
			public:
				ruin::variants::blank head_;
			public:
				constexpr
				trivial_storage()
					: head_()
				{ }
				template < class U >
				explicit constexpr
				trivial_storage(U&&)
					: head_()
				{ }
			};
			template < class Head, class... Tail >
			union trivial_storage<Head, Tail...>
			{
			public:
				ruin::variants::detail::trivial_storage<Tail...> tail_;
				Head head_;
			public:
				constexpr
				trivial_storage()
					: tail_()
				{ }
				constexpr
				trivial_storage(Head const& h)
					: head_(h)
				{ }
				constexpr
				trivial_storage(Head&& h)
					: head_(ruin::forward<Head>(h))
				{ }
				template < class U >
				explicit constexpr
				trivial_storage(U&& u)
					: tail_(ruin::forward<U>(u))
				{ }
			};

			template < class... >
			union nontrivial_storage;
			template <>
			union nontrivial_storage<>
			{
			public:
				ruin::variants::blank head_;
				void* backup_ptr_;
			public:
				nontrivial_storage()
					: head_()
				{ }
				~nontrivial_storage()
				{ }
			};
			template < class Head, class... Tail >
			union nontrivial_storage<Head, Tail...>
			{
			public:
				ruin::variants::detail::nontrivial_storage<Tail...> tail_;
				Head head_;
			public:
				nontrivial_storage()
					: tail_()
				{ }
				~nontrivial_storage()
				{ }
			};

			template < class, class >
			struct target_storage;
			template < class T >
			struct target_storage<ruin::variants::detail::nontrivial_storage<>, T>
			{
				typedef ruin::variants::detail::nontrivial_storage<> type;
			};
			template < class T >
			struct target_storage<ruin::variants::detail::trivial_storage<>, T>
			{
				typedef ruin::variants::detail::trivial_storage<> type;
			};
			template < class Head, class... Tail, class T >
			struct target_storage<ruin::variants::detail::trivial_storage<Head, Tail...>, T>
				: public ruin::mpl::eval_if
					< std::is_same<typename std::decay<T>::type, Head>
					, ruin::mpl::identity<ruin::variants::detail::trivial_storage<Head, Tail...>>
					, ruin::variants::detail::target_storage<ruin::variants::detail::trivial_storage<Tail...>, T>
					>
			{ };
			template < class Head, class... Tail, class T >
			struct target_storage<ruin::variants::detail::nontrivial_storage<Head, Tail...>, T>
				: public ruin::mpl::eval_if
					< std::is_same<typename std::decay<T>::type, Head>
					, ruin::mpl::identity<ruin::variants::detail::nontrivial_storage<Head, Tail...>>
					, ruin::variants::detail::target_storage<ruin::variants::detail::nontrivial_storage<Tail...>, T>
					>
			{ };

			template < class, class >
			struct is_target_storage_of;
			template < class T >
			struct is_target_storage_of<ruin::variants::detail::trivial_storage<>, T>
				: public std::true_type
			{ };
			template < class T >
			struct is_target_storage_of<ruin::variants::detail::nontrivial_storage<>, T>
				: public std::true_type
			{ };
			template < class T, class Head, class... Tail >
			struct is_target_storage_of<ruin::variants::detail::trivial_storage<Head, Tail...>, T>
				: public std::is_same<typename std::decay<T>::type, Head>
			{ };
			template < class T, class Head, class... Tail >
			struct is_target_storage_of<ruin::variants::detail::nontrivial_storage<Head, Tail...>, T>
				: public std::is_same<typename std::decay<T>::type, Head>
			{ };

			template < class Target >
			struct get_storage_t
			{
				template < class Storage, class = typename std::enable_if<!ruin::variants::detail::is_target_storage_of<Storage, Target>::value>::type >
				typename ruin::variants::detail::target_storage<Storage, Target>::type&
				operator()(Storage& storage)
				{
					return (*this)(storage.tail_);
				}
				template < class Storage, class = typename std::enable_if<ruin::variants::detail::is_target_storage_of<Storage, Target>::value>::type >
				Storage&
				operator()(Storage& storage)
				{
					return storage;
				}
				template < class Storage, class = typename std::enable_if<!ruin::variants::detail::is_target_storage_of<Storage, Target>::value>::type >
				constexpr typename ruin::variants::detail::target_storage<Storage, Target>::type const&
				operator()(Storage const& storage)
				{
					return (*this)(storage.tail_);
				}
				template < class Storage, class = typename std::enable_if<ruin::variants::detail::is_target_storage_of<Storage, Target>::value>::type >
				constexpr Storage const&
				operator()(Storage const& storage)
				{
					return storage;
				}
			};
			template < class Target, class Storage >
			constexpr auto
			get_storage(Storage&& storage)
				-> decltype(ruin::variants::detail::get_storage_t<Target>{}(storage))
			{
				return ruin::variants::detail::get_storage_t<Target>{}(storage);
			}

			template < class... >
			struct apply_visitor_t;
			template <>
			struct apply_visitor_t<>
			{
				template < class Visitor, class Visitable >
				constexpr typename std::decay<Visitor>::type::result_type
				operator()(Visitor&& visitor, Visitable&& visitable, int i)
				{
					return visitor(ruin::variants::blank{});
				}
			};
			template < class Head, class... Tail >
			struct apply_visitor_t<Head, Tail...>
			{
				template < class Visitor, class Visitable >
				constexpr typename std::decay<Visitor>::type::result_type
				operator()(Visitor&& visitor, Visitable&& visitable, int i)
				{
					return i == 0
						? visitor(ruin::variants::detail::get_storage<Head>(visitable).head_)
						: ruin::variants::detail::apply_visitor_t<Tail...>{}(ruin::forward<Visitor>(visitor), ruin::forward<Visitable>(visitable), i - 1)
						;
				}
			};
			template < class... >
			struct backup_apply_visitor_t;
			template <>
			struct backup_apply_visitor_t<>
			{
				template < class Visitor, class Visitable >
				constexpr typename std::decay<Visitor>::type::result_type
				operator()(Visitor&& visitor, Visitable&& visitable, int i)
				{
					return visitor(ruin::variants::blank{});
				}
			};
			template < class Head, class... Tail >
			struct backup_apply_visitor_t<Head, Tail...>
			{
				template < class Visitor, class Visitable >
				constexpr typename std::decay<Visitor>::type::result_type
				operator()(Visitor&& visitor, Visitable& visitable, int i)
				{
					return i == 0
						? visitor(*static_cast<Head*>(ruin::variants::detail::get_storage<void>(visitable).backup_ptr_))
						: ruin::variants::detail::backup_apply_visitor_t<Tail...>{}(ruin::forward<Visitor>(visitor), visitable, i - 1)
						;
				}
				template < class Visitor, class Visitable >
				constexpr typename std::decay<Visitor>::type::result_type
				operator()(Visitor&& visitor, Visitable const& visitable, int i)
				{
					return i == 0
						? visitor(*static_cast<Head const*>(ruin::variants::detail::get_storage<void>(visitable).backup_ptr_))
						: ruin::variants::detail::backup_apply_visitor_t<Tail...>{}(ruin::forward<Visitor>(visitor), visitable, i - 1)
						;
				}
			};

			struct destroyer
				: public ruin::variants::static_visitor<>
			{
				template < class T >
				void
				operator()(T& t) const
				{
					t.~T();
				}
				void
				operator()(ruin::variants::blank) const
				{ }
			};
			struct backup_destroyer
				: public ruin::variants::static_visitor<>
			{
				template < class T >
				void
				operator()(T& t) const
				{
					delete ruin::address_of(t);
				}
				void
				operator()(ruin::variants::blank) const
				{ }
			};
			template < class VariantImpl >
			struct constructor
				: public ruin::variants::static_visitor<>
			{
			private:
				VariantImpl& lhs_;
			public:
				constructor(VariantImpl& lhs)
					: lhs_(lhs)
				{ }
			public:
				template < class U >
				void
				operator()(U const& u) const
				{
					lhs_.construct(u);
				}
				void
				operator()(ruin::variants::blank) const
				{ }
			};
			template < class VariantImpl >
			struct move_constructor
				: public ruin::variants::static_visitor<>
			{
			private:
				VariantImpl& lhs_;
			public:
				move_constructor(VariantImpl& lhs)
					: lhs_(lhs)
				{ }
			public:
				template < class U >
				void
				operator()(U& u) const
				{
					lhs_.construct(ruin::forward<U>(u));
				}
				void
				operator()(ruin::variants::blank) const
				{ }
			};

			template < bool, class... Types >
			struct variant_impl;
			template < class... Types >
			struct variant_impl<true, Types...>
			{
			public:
				static constexpr int invalid_index = sizeof...(Types);
				typedef ruin::variants::detail::trivial_storage<Types...> storage_type;
			private:
				storage_type impl_;
				int which_;
			public:
				constexpr
				variant_impl()
					: impl_()
					, which_(invalid_index)
				{ }
				template < class T >
				explicit constexpr
				variant_impl(T&& t)
					: impl_(ruin::forward<T>(t))
					, which_(ruin::variants::detail::find_index<ruin::mpl::list<Types...>, T>::value)
				{ }
				variant_impl(variant_impl&& r) = default;
				variant_impl(variant_impl const& r) = default;
			public:
				variant_impl&
				operator=(variant_impl const& r)
				{
					if(this != &r)
					{
						r.apply_visitor(ruin::variants::detail::constructor<variant_impl>(*this));
					}
					return *this;
				}
				variant_impl&
				operator=(variant_impl&& r)
				{
					if(this != &r)
					{
						r.apply_visitor(ruin::variants::detail::move_constructor<variant_impl>(*this));
					}
					return *this;
				}
				template < class T >
				variant_impl&
				operator=(T&& t)
				{
					construct(ruin::forward<T>(t));
					return *this;
				}
			private:
				void
				indicate_which(int which)
				{
					which_ = which;
				}
				void
				destroy()
				{
					indicate_which(invalid_index);
				}
			public:
				constexpr int
				which() const
				{
					return which_;
				}
				template < class Visitor >
				typename std::decay<Visitor>::type::result_type
				apply_visitor(Visitor&& visitor)
				{
					return ruin::variants::detail::apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which());
				}
				template < class Visitor >
				constexpr typename std::decay<Visitor>::type::result_type
				apply_visitor(Visitor&& visitor) const
				{
					return ruin::variants::detail::apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which());
				}
				template
					< class T
					, int TargetIndex = ruin::variants::detail::find_index<ruin::mpl::list<Types...>, T>::value
					, class TargetType = typename ruin::mpl::at_c<ruin::mpl::list<Types...>, TargetIndex>::type
					>
				void
				construct(T&& t)
				{
					::new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(impl_).head_)) TargetType(ruin::forward<T>(t));
					indicate_which(TargetIndex);
				}
			};
			template < class... Types >
			struct variant_impl<false, Types...>
			{
			public:
				static constexpr int invalid_index = sizeof...(Types);
				static constexpr int fallback_type_index = ruin::mpl::find_if<ruin::mpl::list<Types...>, std::is_nothrow_default_constructible>::value;
				typedef ruin::variants::detail::nontrivial_storage<Types...> storage_type;
			private:
				storage_type impl_;
				int which_;
			public:
				variant_impl()
					: impl_()
					, which_(invalid_index)
				{ }
				template < class T >
				explicit
				variant_impl(T&& t)
					: variant_impl()
				{
					construct(ruin::forward<T>(t));
				}
				variant_impl(variant_impl&& r)
					: variant_impl()
				{
					*this = ruin::move(r);
				}
				variant_impl(variant_impl const& r)
					: variant_impl()
				{
					*this = r;
				}
				~variant_impl()
				{
					destroy();
				}
			public:
				variant_impl&
				operator=(variant_impl const& r)
				{
					if(this != &r)
					{
						r.apply_visitor(ruin::variants::detail::constructor<variant_impl>(*this));
					}
					return *this;
				}
				variant_impl&
				operator=(variant_impl&& r)
				{
					if(this != &r)
					{
						r.apply_visitor(ruin::variants::detail::move_constructor<variant_impl>(*this));
					}
					return *this;
				}
				template < class T >
				variant_impl&
				operator=(T&& t)
				{
					construct(ruin::forward<T>(t));
					return *this;
				}
			private:
				bool
				using_backup() const
				{
					return which_ < 0;
				}
				void
				indicate_which(int which)
				{
					which_ = which;
				}
				void
				indicate_backup_which(int which)
				{
					which_ = -(which + 1);
				}
				void
				destroy()
				{
					if(using_backup())
					{
						apply_visitor(ruin::variants::detail::backup_destroyer{});
					}
					else
					{					
						apply_visitor(ruin::variants::detail::destroyer{});
					}
					indicate_which(invalid_index);
				}
			public:
				int
				which() const
				{
					return using_backup()
						? -(which_ + 1)
						: which_
						;
				}
				template < class Visitor >
				typename std::decay<Visitor>::type::result_type
				apply_visitor(Visitor&& visitor)
				{
					return using_backup()
						? ruin::variants::detail::backup_apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which())
						: ruin::variants::detail::apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which())
						;
				}
				template < class Visitor >
				typename std::decay<Visitor>::type::result_type
				apply_visitor(Visitor&& visitor) const
				{
					return using_backup()
						? ruin::variants::detail::backup_apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which())
						: ruin::variants::detail::apply_visitor_t<Types...>{}(ruin::forward<Visitor>(visitor), impl_, which())
						;
				}
				template < class T >
				void
				construct(T const& rhs)
				{
					construct_helper<T>::construct(*this, rhs);
				}
				template < class T >
				void
				construct(T&& rhs)
				{
					move_construct_helper<T>::construct(*this, ruin::move(rhs));
				}
			private:
				template
					< class T
					, int TargetIndex = ruin::variants::detail::find_index<ruin::mpl::list<Types...>, T>::value
					, class TargetType = typename ruin::mpl::at_c<ruin::mpl::list<Types...>, TargetIndex>::type
					, bool NothrowCopyable = std::is_nothrow_copy_constructible<TargetType>::value
					, bool NothrowMovable = std::is_nothrow_move_constructible<TargetType>::value
					, bool HasFallbackType = fallback_type_index != invalid_index
					>
				struct construct_helper;
				template < class T, int TargetIndex, class TargetType, bool NothrowMovable, bool HasFallbackType >
				struct construct_helper<T, TargetIndex, TargetType, true, NothrowMovable, HasFallbackType>
				{
					static void
					construct(variant_impl& lhs, T const& rhs)
					{
						lhs.destroy();
						::new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(lhs.impl_).head_)) T(rhs);
						lhs.indicate_which(TargetIndex);
					}
				};
				template < class T, int TargetIndex, class TargetType, bool HasFallbackType >
				struct construct_helper<T, TargetIndex, TargetType, false, true, HasFallbackType>
				{
					static void
					construct(variant_impl& lhs, T const& rhs)
					{
						T temp(rhs);
						lhs.destroy();
						::new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(lhs.impl_).head_)) T(ruin::move(temp));
						lhs.indicate_which(TargetIndex);
					}
				};
				template < class T, int TargetIndex, class TargetType >
				struct construct_helper<T, TargetIndex, TargetType, false, false, true>
				{
					static void
					construct(variant_impl& lhs, T const& rhs)
					{
						typedef typename ruin::mpl::at_c<ruin::mpl::list<Types...>, fallback_type_index>::type fallback_type;
						lhs.destroy();
						try
						{
							new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(lhs.impl_).head_)) T(rhs);
						}
						catch(...)
						{
							::new(ruin::address_of(ruin::variants::detail::get_storage<fallback_type>(lhs.impl_).head_)) fallback_type;
							lhs.indicate_which(fallback_type_index);
							throw;
						}
						lhs.indicate_which(TargetIndex);
					}
				};
				template < class T, int TargetIndex, class TargetType >
				struct construct_helper<T, TargetIndex, TargetType, false, false, false>
				{
					static void
					construct(variant_impl& lhs, T const& rhs)
					{
						auto* backup_ptr = new TargetType(ruin::variants::detail::get_storage<T>(lhs.impl_).head_);
						lhs.destroy();
						try
						{
							::new(ruin::address_of(ruin::variants::detail::get_storage<T>(lhs.impl_).head_)) T(rhs);
						}
						catch(...)
						{
							ruin::variants::detail::get_storage<void>(lhs.impl_).backup_ptr_ = backup_ptr;
							lhs.indicate_backup_which(lhs.which());
							throw;
						}
						lhs.indicate_which(TargetIndex);
						delete backup_ptr;
					}
				};
				template
					< class T
					, int TargetIndex = ruin::variants::detail::find_index<ruin::mpl::list<Types...>, T>::value
					, class TargetType = typename ruin::mpl::at_c<ruin::mpl::list<Types...>, TargetIndex>::type
					, bool NothrowMovable = std::is_nothrow_move_constructible<TargetType>::value
					, bool HasFallbackType = fallback_type_index != invalid_index
					>
				struct move_construct_helper;
				template < class T, int TargetIndex, class TargetType, bool HasFallbackType >
				struct move_construct_helper<T, TargetIndex, TargetType, true, HasFallbackType>
				{
					static void
					construct(variant_impl& lhs, T&& rhs)
					{
						lhs.destroy();
						::new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(lhs.impl_).head_)) TargetType(ruin::move(rhs));
						lhs.indicate_which(TargetIndex);
					}
				};
				template < class T, int TargetIndex, class TargetType >
				struct move_construct_helper<T, TargetIndex, TargetType, false, true>
				{
					static void
					construct(variant_impl& lhs, T&& rhs)
					{
						typedef typename ruin::mpl::at_c<ruin::mpl::list<Types...>, fallback_type_index>::type fallback_type;
						lhs.destroy();
						try
						{
							new(ruin::address_of(ruin::variants::detail::get_storage<TargetType>(lhs.impl_).head_)) TargetType(ruin::move(rhs));
						}
						catch(...)
						{
							::new(ruin::address_of(ruin::variants::detail::get_storage<fallback_type>(lhs.impl_).head_)) fallback_type;
							lhs.indicate_which(fallback_type_index);
							throw;
						}
						lhs.indicate_which(TargetIndex);
					}
				};
				template < class T, int TargetIndex, class TargetType >
				struct move_construct_helper<T, TargetIndex, TargetType, false, false>
				{
					static void
					construct(variant_impl& lhs, T&& rhs)
					{
						auto* backup_ptr = new TargetType(ruin::variants::detail::get_storage<T>(lhs.impl_).head_);
						lhs.destroy();
						try
						{
							::new(ruin::address_of(ruin::variants::detail::get_storage<T>(lhs.impl_).head_)) TargetType(ruin::move(rhs));
						}
						catch(...)
						{
							ruin::variants::detail::get_storage<void>(lhs.impl_).backup_ptr_ = backup_ptr;
							lhs.indicate_backup_which(lhs.which());
							throw;
						}
						lhs.indicate_which(TargetIndex);
						delete backup_ptr;
					}
				};
			};
		} // namespace detail

		template < class... Types >
		class variant
			: private ruin::variants::detail::variant_impl
				< ruin::mpl::all_of<ruin::mpl::list<Types...>, std::is_literal_type>::value
				, Types...
				>
		{
		private:
			typedef ruin::variants::detail::variant_impl
				< ruin::mpl::all_of<ruin::mpl::list<Types...>, std::is_literal_type>::value
				, Types...
				> inherited;
		public:
			constexpr
			variant()
				: inherited()
			{ }
			template < class U >
			explicit constexpr
			variant(U&& u)
				: inherited(ruin::forward<U>(u))
			{ }
			variant(variant&& r) = default;
			variant(variant const& r) = default;
		public:
			template < class T, class = typename std::enable_if<!std::is_same<typename std::decay<T>::type, variant>::value>::type >
			variant&
			operator=(T&& t)
			{
				static_cast<inherited&>(*this) = ruin::forward<T>(t);
				return *this;
			}
			variant&
			operator=(variant&& r)
			{
				static_cast<inherited&>(*this) = static_cast<inherited&&>(r);
				return *this;
			}
			variant&
			operator=(variant const& r)
			{
				static_cast<inherited&>(*this) = static_cast<inherited const&>(r);
				return *this;
			}
		public:
			constexpr int
			which() const
			{
				return inherited::which();
			}
			template < class Visitor >
			typename std::decay<Visitor>::type::result_type
			apply_visitor(Visitor&& visitor)
			{
				return inherited::apply_visitor(ruin::forward<Visitor>(visitor));
			}
			template < class Visitor >
			constexpr typename std::decay<Visitor>::type::result_type
			apply_visitor(Visitor&& visitor) const
			{
				return inherited::apply_visitor(ruin::forward<Visitor>(visitor));
			}
			void
			swap(variant& y)
			{
				variant temp(ruin::move(*this));
				*this = ruin::move(y);
				y = ruin::move(temp);
			}
		};

		template < class... Types >
		struct variant_traits<ruin::variants::variant<Types...>>
		{
			static constexpr std::size_t size = sizeof...(Types);
			template < std::size_t I >
			struct element
				: public ruin::mpl::at_c<ruin::mpl::list<Types...>, I>
			{ };
			template < class Visitor >
			static constexpr typename std::decay<Visitor>::type::result_type
			apply_visitor(Visitor&& visitor, ruin::variants::variant<Types...>& visitable)
			{
				return visitable.apply_visitor(ruin::forward<Visitor>(visitor));
			}
			template < class Visitor >
			static constexpr typename std::decay<Visitor>::type::result_type
			apply_visitor(Visitor&& visitor, ruin::variants::variant<Types...> const& visitable)
			{
				return visitable.apply_visitor(ruin::forward<Visitor>(visitor));
			}
			template < class Visitor >
			static constexpr typename std::decay<Visitor>::type::result_type
			apply_visitor(Visitor&& visitor, ruin::variants::variant<Types...>&& visitable)
			{
				return visitable.apply_visitor(ruin::forward<Visitor>(visitor));
			}
		};

		template < class... Types >
		void
		swap(ruin::variants::variant<Types...>& x, ruin::variants::variant<Types...>& y)
		{
			x.swap(y);
		}
	} // namespace variants

	using ruin::variants::swap;
}

#endif // RUIN_VARIANT_VARIANT_HPP_INCLUDED

