//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_CONTAINER_VALARRAY_HPP_INCLUDED
#define RUIN_CONTAINER_VALARRAY_HPP_INCLUDED

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iterator>
#include "ruin/utility/index_tuple.hpp"

namespace ruin
{
	template<class T, std::size_t N>
	class valarray
	{
	public:
		T elems_[N];
	private:
		typedef typename ruin::index_range<0, N>::type range;
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
		explicit constexpr valarray(As&&... as)
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
				: (throw std::out_of_range("valarray<>: index out of range"), elems_[i])
				;
		}
		constexpr const_reference at(difference_type i) const
		{
			return 0 <= i && i < N
				? elems_[i]
				: (throw std::out_of_range("valarray<>: index out of range"), elems_[i])
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
		void swap(valarray& y)
		{
			std::swap(elems_, y.elems_);
		}
	public:
		constexpr valarray operator+(valarray const& r) const
		{
			return plus_impl(*this, r, range{});
		}
		constexpr valarray operator-(valarray const& r) const
		{
			return minus_impl(*this, r, range{});
		}
		constexpr valarray operator*(valarray const& r) const
		{
			return multiplies_impl(*this, r, range{});
		}
		constexpr valarray operator/(valarray const& r) const
		{
			return divides_impl(*this, r, range{});
		}
		constexpr valarray operator%(valarray const& r) const
		{
			return modulus_impl(*this, r, range{});
		}
		constexpr valarray operator+() const
		{
			return unary_plus_impl(*this, range{});
		}
		constexpr valarray operator-() const
		{
			return negate_impl(*this, range{});
		}
		valarray& operator++()
		{
			pre_inc_impl(*this, range{});
			return *this;
		}
		valarray& operator--()
		{
			pre_dec_impl(*this, range{});
			return *this;
		}
		valarray operator++(int)
		{
			return post_inc_impl(*this, range{});
		}
		valarray operator--(int)
		{
				return post_dec_impl(*this, range{});
		}
		valarray& operator+=(valarray const& r)
		{
			addition_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator-=(valarray const& r)
		{
			subtraction_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator*=(valarray const& r)
		{
			multiply_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator/=(valarray const& r)
		{
			divide_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator%=(valarray const& r)
		{
			modulus_assign_impl(*this, r, range{});
			return *this;
		}
		constexpr valarray operator|(valarray const& r) const
		{
			return bitor_impl(*this, r, range{});
		}
		constexpr valarray operator^(valarray const& r) const
		{
			return bitxor_impl(*this, r, range{});
		}
		constexpr valarray operator&(valarray const& r) const
		{
			return bitand_impl(*this, r, range{});
		}
		constexpr valarray operator~() const
		{
			return complement_impl(*this, range{});
		}
		valarray& operator|=(valarray const& r)
		{
			bitor_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator^=(valarray const& r)
		{
			bitxor_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator&=(valarray const& r)
		{
			bitand_assign_impl(*this, r, range{});
			return *this;
		}
		constexpr valarray operator<<(valarray const& r) const
		{
			return left_shift_impl(*this, r, range{});
		}
		constexpr valarray operator>>(valarray const& r) const
		{
			return right_shift_impl(*this, r, range{});
		}
		valarray& operator<<=(valarray const& r)
		{
			left_shift_assign_impl(*this, r, range{});
			return *this;
		}
		valarray& operator>>=(valarray const& r)
		{
			right_shift_assign_impl(*this, r, range{});
			return *this;
		}
		constexpr ruin::valarray<bool, N> operator||(valarray const& r) const
		{
			return logical_or_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N> operator&&(valarray const& r) const
		{
			return logical_and_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N> operator!() const
		{
			return logical_not_impl(*this, range{});
		}
		constexpr ruin::valarray<bool, N> operator==(valarray const& r) const
		{
			return equal_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N> operator!=(valarray const& r) const
		{
			return not_equal_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N> operator<(valarray const& r) const
		{
			return less_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N> operator<=(valarray const& r) const
		{
			return less_equal_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N>
		operator>(valarray const& r) const
		{
			return greater_impl(*this, r, range{});
		}
		constexpr ruin::valarray<bool, N>
		operator>=(valarray const& r) const
		{
			return greater_equal_impl(*this, r, range{});
		}
	private:
		template<std::size_t... Indices>
		static constexpr valarray plus_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] + r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray minus_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] - r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray multiplies_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] * r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray divides_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] / r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray modulus_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] % r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray unary_plus_impl(valarray const& l, ruin::index_tuple<Indices...>)
		{
			return valarray(+l[Indices]...);
		}
		template<std::size_t... Indices>
		static constexpr valarray negate_impl(valarray const& l, ruin::index_tuple<Indices...>)
		{
			return valarray(-l[Indices]...);
		}	
		template<std::size_t... Indices>
		static void pre_inc_impl(valarray& l, ruin::index_tuple<Indices...>)
		{
			ruin::no_op(++l[Indices]...);
		}
		template<std::size_t... Indices>
		static void pre_dec_impl(valarray& l, ruin::index_tuple<Indices...>)
		{
			ruin::no_op(--l[Indices]...);
		}
			template<std::size_t... Indices>
		static valarray post_inc_impl(valarray& l, ruin::index_tuple<Indices...>)
		{
			return valarray(l[Indices]++...);
		}
		template<std::size_t... Indices>
		static valarray post_dec_impl(valarray& l, ruin::index_tuple<Indices...>)
		{
			return valarray(l[Indices]--...);
		}
		template<std::size_t... Indices>
		static void addition_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] += r[Indices])...);
		}
		template<std::size_t... Indices>
		static void subtraction_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] -= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void multiply_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] *= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void divide_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] /= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void modulus_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] %= r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray bitor_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] | r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray bitxor_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] ^ r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray bitand_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] & r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray complement_impl(valarray const& l, ruin::index_tuple<Indices...>)
		{
			return valarray(~l[Indices]...);
		}
		template<std::size_t... Indices>
		static void bitor_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] |= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void bitxor_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] ^= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void bitand_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] &= r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray left_shift_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] << r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr valarray right_shift_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return valarray((l[Indices] >> r[Indices])...);
		}
		template<std::size_t... Indices>
		static void left_shift_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] <<= r[Indices])...);
		}
		template<std::size_t... Indices>
		static void right_shift_assign_impl(valarray& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			ruin::no_op((l[Indices] >>= r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> logical_or_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] || r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> logical_and_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] && r[Indices])...);
		}
		template<std::size_t... Indices> static constexpr ruin::valarray<bool, N>
		logical_not_impl(valarray const& l, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>(!l[Indices]...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> equal_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] == r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> not_equal_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] != r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> less_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] < r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> less_equal_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] <= r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> greater_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] > r[Indices])...);
		}
		template<std::size_t... Indices>
		static constexpr ruin::valarray<bool, N> greater_equal_impl(valarray const& l, valarray const& r, ruin::index_tuple<Indices...>)
		{
			return ruin::valarray<bool, N>((l[Indices] >= r[Indices])...);
		}
	};
}

#endif // RUIN_CONTAINER_VALARRAY_HPP_INCLUDED
