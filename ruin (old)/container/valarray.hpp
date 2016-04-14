
#ifndef RUIN_CONTAINER_VALARRAY_HPP_INCLUDED
#define RUIN_CONTAINER_VALARRAY_HPP_INCLUDED

#include <cstddef>
#include <stdexcept>
#include "ruin/iterator/reverse_iterator.hpp"
#include "ruin/utility/index_tuple.hpp"
#include "ruin/utility/forward.hpp"
#include "ruin/utility/move.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin
{
	namespace container
	{
		template < class T, std::size_t N >
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
			typedef ruin::iterator::reverse_iterator<iterator> reverse_iterator;
			typedef ruin::iterator::reverse_iterator<const_iterator> const_reverse_iterator;
		public:
			template < class... As >
			explicit constexpr
			valarray(As&&... as)
				: elems_{ruin::forward<As>(as)...}
			{ }
		public:
			reference
			operator[](difference_type i)
			{
				return elems_[i];
			}
			constexpr const_reference
			operator[](difference_type i) const
			{
				return elems_[i];
			}
			reference
			at(difference_type i)
			{
				return 0 <= i && i < N
					? elems_[i]
					: (throw std::out_of_range("valarray<>: index out of range"), elems_[i])
					;
			}
			constexpr const_reference
			at(difference_type i) const
			{
				return 0 <= i && i < N
					? elems_[i]
					: (throw std::out_of_range("valarray<>: index out of range"), elems_[i])
					;
			}
			iterator
			begin()
			{
				return elems_;
			}
			constexpr const_iterator
			begin() const
			{
				return elems_;
			}
			constexpr const_iterator
			cbegin() const
			{
				return elems_;
			}
			iterator
			end()
			{
				return elems_ + N;
			}
			constexpr const_iterator
			end() const
			{
				return elems_ + N;
			}
			constexpr const_iterator
			cend() const
			{
				return elems_ + N;
			}
			reverse_iterator
			rbegin()
			{
				return reverse_iterator(end());
			}
			constexpr const_reverse_iterator
			rbegin() const
			{
				return const_reverse_iterator(end());
			}
			constexpr const_reverse_iterator
			crbegin() const
			{
				return const_reverse_iterator(end());
			}
			reverse_iterator
			rend()
			{
				return reverse_iterator(begin());
			}
			constexpr const_reverse_iterator
			rend() const
			{
				return const_reverse_iterator(begin());
			}
			constexpr const_reverse_iterator
			crend() const
			{
				return const_reverse_iterator(begin());
			}
			reference
			front()
			{
				return elems_[0];
			}
			constexpr const_reference
			front() const
			{
				return elems_[0];
			}
			reference
			back()
			{
				return elems_[N - 1];
			}
			constexpr const_reference
			back() const
			{
				return elems_[N - 1];
			}
			constexpr size_type
			size() const
			{
				return N;
			}
			constexpr bool
			empty() const
			{
				return false;
			}
			constexpr size_type
			max_size() const
			{
				return N;
			}
			T*
			data()
			{
				return elems_;
			}
			T const*
			data() const
			{
				return elems_;
			}
			T*
			c_array()
			{
				return elems_;
			}
			void
			swap(valarray& y)
				noexcept(noexcept(ruin::swap(valarray::elems_, y.elems_)))
			{
				ruin::swap(elems_, y.elems_);
			}
		private:
			template < std::size_t... Indexes >
			static constexpr valarray
			plus(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] + rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			minus(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] - rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			multiplies(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] * rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			divides(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] / rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			modulus(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] % rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			unary_plus(valarray const& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(+lhs[Indexes]...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			negate(valarray const& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(-lhs[Indexes]...);
			}	
			template < std::size_t... Indexes >
			static void
			pre_inc(valarray& lhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op(++lhs[Indexes]...);
			}
			template < std::size_t... Indexes >
			static void
			pre_dec(valarray& lhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op(--lhs[Indexes]...);
			}
			template < std::size_t... Indexes >
			static valarray
			post_inc(valarray& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(lhs[Indexes]++...);
			}
			template < std::size_t... Indexes >
			static valarray
			post_dec(valarray& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(lhs[Indexes]--...);
			}
			template < std::size_t... Indexes >
			static void
			addition_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] += rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			subtraction_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] -= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			multiply_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] *= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			divide_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] /= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			modulus_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] %= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			bitor_(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] | rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			bitxor_(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] ^ rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			bitand_(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] & rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			complement(valarray const& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(~lhs[Indexes]...);
			}
			template < std::size_t... Indexes >
			static void
			bitor_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] |= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			bitxor_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] ^= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			bitand_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] &= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			left_shift(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] << rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr valarray
			right_shift(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] >> rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			left_shift_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] <<= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static void
			right_shift_assign(valarray& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				ruin::no_op((lhs[Indexes] >>= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			logical_or(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] || rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			logical_and(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] && rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			logical_not(valarray const& lhs, ruin::index_tuple<Indexes...>)
			{
				return valarray(!lhs[Indexes]...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			equal(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] == rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			not_equal(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] != rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			less(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] < rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			less_equal(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] <= rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			greater(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] > rhs[Indexes])...);
			}
			template < std::size_t... Indexes >
			static constexpr ruin::container::valarray<bool, N>
			greater_equal(valarray const& lhs, valarray const& rhs, ruin::index_tuple<Indexes...>)
			{
				return valarray((lhs[Indexes] >= rhs[Indexes])...);
			}
		public:
			constexpr valarray
			operator+(valarray const& r) const
			{
				return plus(*this, r, range{});
			}
			constexpr valarray
			operator-(valarray const& r) const
			{
				return minus(*this, r, range{});
			}
			constexpr valarray
			operator*(valarray const& r) const
			{
				return multiplies(*this, r, range{});
			}
			constexpr valarray
			operator/(valarray const& r) const
			{
				return divides(*this, r, range{});
			}
			constexpr valarray
			operator%(valarray const& r) const
			{
				return modulus(*this, r, range{});
			}
			constexpr valarray
			operator+() const
			{
				return unary_plus(*this, range{});
			}
			constexpr valarray
			operator-() const
			{
				return negate(*this, range{});
			}
			valarray&
			operator++()
			{
				pre_inc(*this, range{});
				return *this;
			}
			valarray&
			operator--()
			{
				pre_dec(*this, range{});
				return *this;
			}
			valarray
			operator++(int)
			{
				return post_inc(*this, range{});
			}
			valarray
			operator--(int)
			{
				return post_dec(*this, range{});
			}
			valarray&
			operator+=(valarray const& r)
			{
				addition_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator-=(valarray const& r)
			{
				subtraction_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator*=(valarray const& r)
			{
				multiply_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator/=(valarray const& r)
			{
				divide_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator%=(valarray const& r)
			{
				modulus_assign(*this, r, range{});
				return *this;
			}
			constexpr valarray
			operator|(valarray const& r) const
			{
				return bitor_(*this, r, range{});
			}
			constexpr valarray
			operator^(valarray const& r) const
			{
				return bitxor_(*this, r, range{});
			}
			constexpr valarray
			operator&(valarray const& r) const
			{
				return bitand_(*this, r, range{});
			}
			constexpr valarray
			operator~() const
			{
				return complement(*this, range{});
			}
			valarray&
			operator|=(valarray const& r)
			{
				bitor_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator^=(valarray const& r)
			{
				bitxor_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator&=(valarray const& r)
			{
				bitand_assign(*this, r, range{});
				return *this;
			}
			constexpr valarray
			operator<<(valarray const& r) const
			{
				return left_shift(*this, r, range{});
			}
			constexpr valarray
			operator>>(valarray const& r) const
			{
				return right_shift(*this, r, range{});
			}
			valarray&
			operator<<=(valarray const& r)
			{
				left_shift_assign(*this, r, range{});
				return *this;
			}
			valarray&
			operator>>=(valarray const& r)
			{
				right_shift_assign(*this, r, range{});
				return *this;
			}
			constexpr ruin::container::valarray<bool, N>
			operator||(valarray const& r) const
			{
				return logical_or(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator&&(valarray const& r) const
			{
				return logical_and(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator!() const
			{
				return logical_not(*this, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator==(valarray const& r) const
			{
				return equal(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator!=(valarray const& r) const
			{
				return not_equal(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator<(valarray const& r) const
			{
				return less(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator<=(valarray const& r) const
			{
				return less_equal(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator>(valarray const& r) const
			{
				return greater(*this, r, range{});
			}
			constexpr ruin::container::valarray<bool, N>
			operator>=(valarray const& r) const
			{
				return greater_equal(*this, r, range{});
			}
		};

		template < class T, std::size_t N >
		void
		swap(ruin::container::valarray<T, N>& x, ruin::container::valarray<T, N>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace container

	using ruin::container::swap;
}

#endif // RUIN_CONTAINER_VALARRAY_HPP_INCLUDED
