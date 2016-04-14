//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_CONTAINER_ARRAY_HPP_INCLUDED
#define RUIN_CONTAINER_ARRAY_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace ruin
{
	template<class T, std::size_t N>
	class array
	{
	public:
		T elems_[N];
	public:
		typedef T value_type;
		typedef T& reference;
		typedef T const& const_reference;
		typedef T* pointer;
		typedef T const* const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* iterator;
		typedef T const* const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;	
	public:
		template<class... As>
		explicit constexpr array(As&&... as)
			: elems_{std::forward<As>(as)...}
		{ }
	public:
		reference operator[](difference_type i)
		{
			return elems_[i];
		}
		constexpr const_reference operator[](difference_type i) const
		{
			return elems_[i];
		}
		reference at(difference_type i)
		{
			return 0 <= i && i < N
				? elems_[i]
				: (throw std::out_of_range("array<>: index out of range"), elems_[i])
				;
		}
		constexpr const_reference at(difference_type i) const
		{
			return 0 <= i && i < N
				? elems_[i]
				: (throw std::out_of_range("array<>: index out of range"), elems_[i])
				;
		}
		iterator begin()
		{
			return elems_;
		}
		constexpr const_iterator begin() const
		{
			return elems_;
		}
		constexpr const_iterator cbegin() const
		{
			return elems_;
		}
		iterator end()
		{
			return elems_ + N;
		}
		constexpr const_iterator end() const
		{
			return elems_ + N;
		}
		constexpr const_iterator cend() const
		{
			return elems_ + N;
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
		constexpr const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}
		constexpr const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		constexpr const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}
		constexpr const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		}
		reference front()
		{
			return elems_[0];
		}
		constexpr const_reference front() const
		{
			return elems_[0];
		}
		reference back()
		{
			return elems_[N - 1];
		}
		constexpr const_reference back() const
		{
			return elems_[N - 1];
		}
		constexpr size_type size() const
		{
			return N;
		}
		constexpr bool empty() const
		{
			return false;
		}
		constexpr size_type max_size() const
		{
			return N;
		}
		T* data()
		{
			return elems_;
		}
		T const* data() const
		{
			return elems_;
		}
		T* c_array()
		{
			return elems_;
		}
		void swap(array& y)
		{
			std::swap(elems_, y.elems_);
		}
	};
}

#endif // RUIN_CONTAINER_ARRAY_HPP_INCLUDED
