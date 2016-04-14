//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_OPTIONAL_OPTIONAL_HPP_INCLUDED
#define RUIN_OPTIONAL_OPTIONAL_HPP_INCLUDED

#include <type_traits>
#include <utility>

namespace ruin
{
	struct default_construct_t
	{ };
	static constexpr ruin::default_construct_t no_arg{};
	
	namespace detail
	{
		template<class T, bool = std::is_reference<T>::value, bool = std::has_trivial_copy_constructor<T>::value, bool = std::is_trivially_destructible<T>::value>
		struct optional_impl;
		template<class T>
		struct optional_impl<T, true, true, true>
		{
		private:
			typedef typename std::add_pointer<T>::type pointer;
		private:
			pointer p_;
		public:
			constexpr optional_impl()
				: p_(nullptr)
			{ }
			explicit constexpr optional_impl(T&& t)
				: p_(&t)
			{ }
			optional_impl(optional_impl const&) = default;
		public:
			constexpr bool is_initialized() const
			{
				return p_;
			}
			constexpr pointer get_ptr() const
			{
				return p_;
			}
			constexpr T get() const
			{
				return *p_;
			}
			void construct(T&& t)
			{
				p_ = &t;
			}
			void dispose()
			{
				p_ = nullptr;
			}
			void swap(optional_impl& y) noexcept
			{
				std::swap(p_, y.p_);
			}
		};
		template<class T>
		struct optional_impl<T, false, true, true>
		{
		private:
			union storage_type
			{
			public:
				T data_;
				struct { } empty_;
			public:
				constexpr storage_type()
					: empty_()
				{ }
				constexpr storage_type(ruin::default_construct_t)
					: data_()
				{ }
				template < class... As >
				explicit constexpr storage_type(As&&... as)
					: data_(std::forward<As>(as)...)
				{ }
			} storage_;
			bool initialized_;
		public:
			constexpr optional_impl()
				: storage_(), initialized_(false)
			{ }
			template < class... As >
			explicit constexpr optional_impl(As&&... as)
				: storage_(std::forward<As>(as)...), initialized_(true)
			{ }
			optional_impl(optional_impl const&) = default;
		public:
			constexpr bool is_initialized() const
			{
				return initialized_;
			}
			T* get_ptr()
			{
				return &storage_.data_;
			}
			constexpr T const* get_ptr() const
			{
				return &storage_.data_;
			}
			T& get()
			{
				return storage_.data_;
			}
			constexpr T const& get() const
			{
				return storage_.data_;
			}
			template<class... As>
			void construct(As&&... as)
			{
				dispose();
				::new(&storage_) T(ruin::forward<As>(as)...);
				initialized_ = true;
			}
			void dispose()
			{
				initialized_ = false;
			}
			void swap(optional_impl& other)
			{
				std::swap(storage_, other.storage_);
				std::swap(initialized_, other.initialized_);
			}
		};
		template<class T>
		struct optional_impl<T, false, true, false>
		{
		private:
			typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
			bool initialized_;
		public:
			constexpr optional_impl()
				: storage_(), initialized_(false)
			{ }
			optional_impl(ruin::default_construct_t)
				: initialized_(true)
			{
				::new(&storage_) T();
			}
			template < class... As >
			explicit optional_impl(As&&... as)
				: initialized_(true)
			{
				::new(&storage_) T(std::forward<As>(as)...);
			}
			optional_impl(optional_impl const& r)
			{
				if(r.is_initialized())
				{
					::new(&storage_) T(r.get());
					initialized_ = true;
				}
				else
				{
					initialized_ = false;
				}
			}
		public:
			bool is_initialized() const
			{
				return initialized_;
			}
			T* get_ptr()
			{
				return reinterpret_cast<T*>(&storage_);
			}
			T const* get_ptr() const
			{
				return reinterpret_cast<T const*>(&storage_);
			}
			T& get()
			{
				return *get_ptr();
			}
			T const& get() const
			{
				return *get_ptr();
			}
			template<class... As>
			void construct(As&&... as)
			{
				dispose();
				::new(&storage_) T(ruin::forward<As>(as)...);
				initialized_ = true;
			}
			void dispose()
			{
				if(is_initialized())
				{
					get().~T();
					initialized_ = false;
				}
			}
			void swap(optional_impl& other)
			{
				std::swap(storage_, other.storage_);
				std::swap(initialized_, other.initialized_);
			}
		};
		template<class T>
		struct optional_impl<T, false, false, true>
		{
		private:
			union storage_type
			{
			public:
				T data_;
				struct { } empty_;
			public:
				storage_type()
					: empty_()
				{ }
				storage_type(ruin::default_construct_t)
					: data_()
				{ }
				template < class... As >
				explicit storage_type(As&&... as)
					: data_(std::forward<As>(as)...)
				{ }
			} storage_;
			bool initialized_;
		public:
			optional_impl()
				: storage_(), initialized_(false)
			{ }
			template < class... As >
			explicit optional_impl(As&&... as)
				: storage_(std::forward<As>(as)...), initialized_(true)
			{ }
			optional_impl(optional_impl const& r)
			{
				if(r.is_initialized())
				{
					construct(r.get());
				}
			}
			optional_impl(optional_impl&& r)
			{
				if(r.is_initialized())
				{
					construct(std::move(r.get()));
				}
			}
		public:
			bool is_initialized() const
			{
				return initialized_;
			}
			T* get_ptr()
			{
				return &storage_.data_;
			}
			T const* get_ptr() const
			{
				return &storage_.data_;
			}
			T& get()
			{
				return storage_.data_;
			}
			T const& get() const
			{
				return storage_.data_;
			}
			template<class... As>
			void construct(As&&... as)
			{
				dispose();
				::new(&storage_) T(ruin::forward<As>(as)...);
				initialized_ = true;
			}
			void dispose()
			{
				if(is_initialized())
				{
					initialized_ = false;
				}
			}
			void swap(optional_impl& other)
			{
				if(is_initialized() && other.is_initialized())
				{
					using std::swap;
					swap(get(), other.get());
				}
				else if(is_initialized())
				{
					construct(std::move(get()));
					dispose();
				}
				else if(other.is_initialized())
				{
					construct(std::move(other.get()));
					other.dispose();
				}
			}
		};
		template<class T>
		struct optional_impl<T, false, false, false>
		{
		private:
			typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
			bool initialized_;
		public:
			optional_impl()
				: initialized_(false)
			{ }
			optional_impl(ruin::default_construct_t)
				: initialized_(true)
			{
				::new(&storage_) T();
			}
			template < class... As >
			explicit optional_impl(As&&... as)
				: initialized_(true)
			{
				::new(&storage_) T(std::forward<As>(as)...);
			}
			optional_impl(optional_impl const& r)
			{
				if(r.is_initialized())
				{
					::new(&storage_) T(r.get());
					initialized_ = true;
				}
				else
				{
					initialized_ = false;
				}
			}
			~optional_impl()
			{
				dispose();
			}
		public:
			bool is_initialized() const
			{
				return initialized_;
			}
			T* get_ptr()
			{
				return reinterpret_cast<T*>(&storage_);
			}
			T const* get_ptr() const
			{
				return reinterpret_cast<T const*>(&storage_);
			}
			T& get()
			{
				return *get_ptr();
			}
			T const& get() const
			{
				return *get_ptr();
			}
			template<class... As>
			void construct(As&&... as)
			{
				dispose();
				::new(&storage_) T(std::forward<As>(as)...);
				initialized_ = true;
			}
			void dispose()
			{
				if(is_initialized())
				{
					get().~T();
					initialized_ = false;
				}
			}
			void swap(optional_impl& other)
			{
				if(is_initialized() && other.is_initialized())
				{
					using std::swap;
					swap(get(), other.get());
				}
				else if(is_initialized())
				{
					construct(ruin::std(get()));
					dispose();
				}
				else if(other.is_initialized())
				{
					construct(ruin::std(other.get()));
					other.dispose();
				}
			}
		};
	}
	
	struct none_t
	{ };
	static constexpr ruin::none_t none{};
	
	template < class T >
	class optional
	{
	private:
		ruin::detail::optional_impl<T> impl_;
	public:
		typedef T element_type;
		typedef typename std::remove_cv<T>::type value_type;
		typedef typename std::remove_reference<T>::type object_type;
		typedef T& reference;
		typedef T const& const_reference;
		typedef object_type* pointer;
		typedef object_type const* const_pointer;
	public:
		constexpr optional()
			: impl_()
		{ }
		constexpr optional(ruin::none_t)
			: impl_()
		{ }
		template < class... As >
		explicit constexpr optional(As&&... as)
			: impl_(std::forward<As>(as)...)
		{ }
		constexpr optional(optional const& r)
			: impl_(r.impl_)
		{ }
	public:
		optional& operator=(ruin::none_t)
		{
			optional().swap(*this);
			return *this;
		}
		optional& operator=(T t)
		{
			optional(t).swap(*this);
			return *this;
		}
		optional& operator=(optional r)
		{
			r.swap(*this);
			return *this;
		}
	public:
		constexpr bool is_initialized() const
		{
			return impl_.is_initialized();
		}
		pointer get_ptr()
		{
			return is_initialized() ? impl_.get_ptr() : throw;
		}
		constexpr const_pointer get_ptr() const
		{
			return is_initialized() ? impl_.get_ptr() : throw;
		}
		reference get()
		{
			return is_initialized() ? impl_.get() : throw, impl_.get();
		}
		constexpr const_reference get() const
		{
			return is_initialized() ? impl_.get_ptr() : throw, impl_.get();
		}
		reference operator*()
		{
			return get();
		}
		constexpr const_reference operator*() const
		{
			return get();
		}
		pointer operator->()
		{
			return get_ptr();
		}
		constexpr const_pointer operator->() const
		{
			return get_ptr();
		}
		explicit constexpr operator bool() const
		{
			return is_initialized();
		}
		constexpr bool operator!() const
		{
			return !is_initialized();
		}
		void swap(optional& y)
		{
			impl_.swap(y.impl_);
		}
	};
}

#endif // RUIN_OPTIONAL_OPTIONAL_HPP_INCLUDED

