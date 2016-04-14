//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_CONTAINER_BITSET_HPP_INCLUDED
#define RUIN_CONTAINER_BITSET_HPP_INCLUDED

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>
#include "ruin/iterator/index_iterator.hpp"
#include "ruin/iterator/reverse_iterator.hpp"
#include "ruin/operators/equality_comparable.hpp"
#include "ruin/operators/less_than_comparable.hpp"
#include "ruin/utility/index_tuple.hpp"

namespace ruin
{
	namespace detail
	{
		static constexpr std::size_t hamming_weight[]
			= {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
		template<std::size_t Nw>
		class bitset_base
		{
		public:
			typedef unsigned int word_type;
		public:
			word_type w_[Nw];
		public:
			constexpr bitset_base()
				: w_{}
			{ }
			template < class... Words >
			explicit constexpr bitset_base(Words... words)
				: w_{words...}
			{ }
		public:
			static constexpr std::size_t whichword(std::size_t pos)
			{
				return pos / std::numeric_limits<word_type>::digits;
			}
			static constexpr std::size_t whichbyte(std::size_t pos)
			{
				return (pos % std::numeric_limits<word_type>::digits) / std::numeric_limits<unsigned char>::digits;
			}
			static constexpr std::size_t whichbit(std::size_t pos)
			{
				return pos % std::numeric_limits<word_type>::digits;
			}
			static constexpr word_type maskbit(std::size_t pos)
			{
				return static_cast<word_type>(1) << whichbit(pos);
			}
			word_type& getword(std::size_t pos)
			{
				return w_[whichword(pos)];
			}
			constexpr word_type getword(std::size_t pos) const
			{
				return w_[whichword(pos)];
			}
			template
				< std::size_t Nb
				, std::size_t Shift = Nb % std::numeric_limits<word_type>::digits != 0
					? std::numeric_limits<word_type>::digits - Nb % std::numeric_limits<word_type>::digits
					: 0
				>
			constexpr word_type hiword() const
			{
				return w_[Nw - 1] & (~static_cast<word_type>(0) >> Shift);
			}
		private:
			typedef typename ruin::index_range<0, Nw>::type range;
			template<std::size_t... Indices>
			static constexpr bitset_base and_expand(bitset_base const& lhs, bitset_base const& rhs, ruin::index_tuple<Indices...>)
			{
				return bitset_base((lhs.w_[Indices] & rhs.w_[Indices])...);
			}
			template<std::size_t... Indices>
			static constexpr bitset_base or_expand(bitset_base const& lhs, bitset_base const& rhs, ruin::index_tuple<Indices...>)
			{
				return bitset_base((lhs.w_[Indices] | rhs.w_[Indices])...);
			}
			template<std::size_t... Indices>
			static constexpr bitset_base xor_expand(bitset_base const& lhs, bitset_base const& rhs, ruin::index_tuple<Indices...>)
			{
				return bitset_base((lhs.w_[Indices] ^ rhs.w_[Indices])...);
			}
			template<std::size_t... Indices>
			static constexpr bitset_base complement_expand(bitset_base const& lhs, ruin::index_tuple<Indices...>)
			{
				return bitset_base(~lhs.w_[Indices]...);
			}
			template<std::size_t... Indices>
			static constexpr bitset_base left_shift_expand(bitset_base const& lhs, std::size_t wshift, std::size_t offset, ruin::index_tuple<Indices...>)
			{
				return bitset_base
					((
						(Indices >= wshift ? lhs.w_[Indices - wshift]
							<< offset : static_cast<word_type>(0))
						| (Indices > wshift ? lhs.w_[Indices - wshift - 1]
							>> (std::numeric_limits<word_type>::digits - offset) : static_cast<word_type>(0))
					)...);
			}
			template<std::size_t... Indices>
			static constexpr bitset_base right_shift_expand(bitset_base const& lhs, std::size_t wshift, std::size_t offset, std::size_t limit, ruin::index_tuple<Indices...>)
			{
				return bitset_base
					((
						(Indices <= limit ? lhs.w_[Indices + wshift]
							>> offset : static_cast<word_type>(0))
						| (Indices < limit ? lhs.w_[Indices + wshift + 1]
							<< (std::numeric_limits<word_type>::digits - offset) : static_cast<word_type>(0))
					)...);
			}
		protected:
			constexpr bitset_base and_impl(bitset_base const& r) const
			{
				return and_expand(*this, r, range{});
			}
			constexpr bitset_base or_impl(bitset_base const& r) const
			{
				return or_expand(*this, r, range{});
			}
			constexpr bitset_base xor_impl(bitset_base const& r) const
			{
				return xor_expand(*this, r, range{});
			}
			constexpr bitset_base complement_impl() const
			{
				return complement_expand(*this, range{});
			}
			constexpr bitset_base left_shift_impl(std::size_t shift) const
			{
				return shift > 0
					? left_shift_expand
						( *this
						, shift / std::numeric_limits<word_type>::digits
						, shift % std::numeric_limits<word_type>::digits
						, range{}
						)
					: *this
					;
			}
			constexpr bitset_base right_shift_impl(std::size_t shift) const
			{
				return shift > 0
					? right_shift_expand
						( *this
						, shift / std::numeric_limits<word_type>::digits
						, shift % std::numeric_limits<word_type>::digits
						, Nw - shift / std::numeric_limits<word_type>::digits - 1
						, range{}
						)
					: *this
					;
			}
			void and_eq_impl(bitset_base const& r)
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] &= r.w_[i];
				}
				}
			void or_eq_impl(bitset_base const& r)
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] |= r.w_[i];
				}
			}
			void xor_eq_impl(bitset_base const& r)
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] ^= r.w_[i];
				}
			}
			void left_shift_eq_impl(std::size_t shift)
			{
				if(shift == 0)
				{
					return;
				}
				std::size_t const wshift = shift / std::numeric_limits<word_type>::digits;
				std::size_t const offset = shift % std::numeric_limits<word_type>::digits;
				if(offset == 0)
				{
					for(std::size_t i = Nw - 1 ; i >= wshift ; --i)
					{
							w_[i] = w_[i - wshift];
					}
				}
				else
				{
					std::size_t const sub_offset = std::numeric_limits<word_type>::digits - offset;
					for(std::size_t i = Nw - 1 ; i > wshift ; --i)
					{
						w_[i] = (w_[i - wshift] << offset) | (w_[i - wshift - 1] >> sub_offset);
					}
					w_[wshift] = w_[0] << offset;
				}
			}
			void right_shift_eq_impl(std::size_t shift)
			{
				if(shift == 0)
				{
					return;
				}
				std::size_t const wshift = shift / std::numeric_limits<word_type>::digits;
				std::size_t const offset = shift % std::numeric_limits<word_type>::digits;
				std::size_t const limit = Nw - wshift - 1;
				if(offset == 0)
				{
					for(std::size_t i = 0 ; i <= limit ; ++i)
					{
						w_[i] = w_[i + shift];
					}
				}
				else
				{
					std::size_t const sub_offset = std::numeric_limits<word_type>::digits - offset;
					for(std::size_t i = 0 ; i < limit ; ++i)
					{
						w_[i] = (w_[i + wshift] >> offset) | (w_[i + shift + 1] << sub_offset);
					}
					w_[limit] = w_[Nw - 1] >> offset;
				}
			}
			void flip_impl(bitset_base const& r)
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] = ~w_[i];
				}
			}
			void set_impl()
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] = ~static_cast<word_type>(0);
				}
			}
			void reset_impl()
			{
				for(std::size_t i = 0 ; i < Nw ; ++i)
				{
					w_[i] = static_cast<word_type>(0);
				}
			}
		private:
			template<std::size_t Nb>
			static constexpr bool is_less_rec(bitset_base const& lhs, bitset_base const& rhs, std::size_t pos = Nw - 2)
			{
				return pos > 0
					? lhs.w_[pos] < rhs.w_[pos] || (lhs.w_[pos] == rhs.w_[pos] && is_less_rec<Nb>(lhs, rhs, pos - 1))
					: lhs.w_[0] < rhs.w_[0]
					;
			}
		protected:
			template<std::size_t Nb>
			constexpr bool is_less_impl(bitset_base const& rhs) const
			{
				return hiword<Nb>() < rhs.hiword<Nb>() || (hiword<Nb>() == rhs.hiword<Nb>() && is_less_rec<Nb>(*this, rhs));
			}
			template<std::size_t Nb>
			constexpr bool is_equal_impl(bitset_base const& rhs, std::size_t pos = 0) const
			{
				return pos < Nw - 1
					? w_[pos] == rhs.w_[pos] && is_equal_impl<Nb>(rhs, pos + 1)
					: hiword<Nb>() == rhs.hiword<Nb>()
					;
			}
			template<std::size_t Nb>
			constexpr bool all_impl(std::size_t pos = 0) const
			{
				return pos < Nw - 1
					? w_[pos] != ~static_cast<word_type>(0)
						? false
						: all_impl<Nb>(pos + 1)
					: hiword<Nb>() == ~static_cast<word_type>(0)
						>> (Nw * std::numeric_limits<word_type>::digits - Nb)
					;
			}
			template<std::size_t Nb>
			constexpr bool any_impl(std::size_t pos = 0) const
			{
				return pos < Nw - 1
					? w_[pos] != static_cast<word_type>(0)
						? true
						: any_impl<Nb>(pos + 1)
					: hiword<Nb>() != static_cast<word_type>(0)
					;
			}
		private:
			static constexpr std::size_t popcount(word_type x, std::size_t count = 0)
			{
				return x != static_cast<word_type>(0)
					? popcount(x >> 4, count + ruin::detail::hamming_weight[x & 0xf])
					: count
					;
			}
		protected:
			template<std::size_t Nb>
			constexpr std::size_t count_impl(std::size_t pos = 0, std::size_t iter = 0) const
			{
				return pos < Nw - 1
					? count_impl<Nb>(pos + 1, iter + popcount(w_[pos]))
					: iter + popcount(hiword<Nb>())
					;
			}
			void swap(bitset_base& y) noexcept
			{
				std::swap(w_, y.w_);
			}
		};
		template<std::size_t Nb>
		struct bitset_word
		{
			static constexpr std::size_t value
				= Nb / std::numeric_limits<unsigned int>::digits
				+ (Nb % std::numeric_limits<unsigned int>::digits == 0 ? 0 : 1)
				;
		};
	} // namespace detail
	template<std::size_t Nb>
	class bitset
		: private ruin::detail::bitset_base<ruin::detail::bitset_word<Nb>::value>
		, private ruin::equality_comparable<ruin::bitset<Nb>>
		, private ruin::less_than_comparable<ruin::bitset<Nb>>
	{
	private:
		typedef ruin::detail::bitset_base<ruin::detail::bitset_word<Nb>::value> base_type;
	private:
		typedef unsigned int word_type;
	public:
		typedef bool value_type;
		class reference
		{
			friend class bitset;
		private:
			word_type* wp_;
			std::size_t pos_;
		public:
			reference() = delete;
			reference(bitset& r, std::size_t pos)
				: wp_(&r.getword(pos))
				, pos_(base_type::whichbit(pos))
			{ }
		public:
			reference& operator=(bool x)
			{
				if(x)
				{
					*wp_ |= base_type::maskbit(pos_);
				}
				else
				{
					*wp_ &= ~base_type::maskbit(pos_);
				}
				return *this;
			}
			reference& operator=(reference const& r)
			{
				if(*r.wp_ & base_type::maskbit(r.pos_))
				{
					*wp_ |= base_type::maskbit(pos_);
				}
				else
				{
					*wp_ &= ~base_type::maskbit(pos_);
				}
				return *this;
			}
			operator bool() const
			{
				return (*wp_ & base_type::maskbit(pos_)) != 0;
			}
		};
		typedef bool const_reference;
		typedef void pointer;
		typedef void const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef ruin::index_iterator<bitset&> iterator;
		typedef ruin::index_iterator<bitset const&> const_iterator;
		typedef ruin::reverse_iterator<iterator> reverse_iterator;
		typedef ruin::reverse_iterator<const_iterator> const_reverse_iterator;
	public:
		constexpr bitset()
			: base_type()
		{ }
		template < class... Words >
		explicit constexpr bitset(Words... words)
			: base_type(words...)
		{ }
	public:
		reference operator[](difference_type i)
		{
			return reference(*this, i);
		}
		constexpr const_reference operator[](difference_type i) const
		{
			return (base_type::getword(i) & base_type::maskbit(i)) != static_cast<word_type>(0);
		}
		reference at(difference_type i)
		{
			return i < Nb
				? reference(*this, i)
				: throw std::out_of_range("bitset<>: index out of range")
				;
		}
		constexpr const_reference at(difference_type i) const
		{
			return i < Nb
				? (base_type::getword(i) & base_type::maskbit(i)) != static_cast<word_type>(0)
				: throw std::out_of_range("bitset<>: index out of range")
				;
		}
		iterator begin()
		{
			return iterator(*this, 0);
		}
		constexpr const_iterator begin() const
		{
			return const_iterator(*this, 0);
		}
		iterator cbegin() const
		{
			return const_iterator(*this, 0);
		}
		iterator end()
		{
			return iterator(*this, Nb);
		}
		constexpr const_iterator end() const
		{
			return const_iterator(*this, Nb);
		}
		iterator cend() const
		{
				return const_iterator(*this, Nb);
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
			return (*this)[0];
		}
		constexpr const_reference front() const
		{
			return (*this)[0];
		}
		reference back()
		{
			return (*this)[Nb - 1];
		}
		constexpr const_reference back() const
		{
			return (*this)[Nb - 1];
		}
		constexpr size_type size() const
		{
			return Nb;
		}
		constexpr bool empty() const
		{
			return false;
		}
		constexpr size_type max_size() const
		{
			return Nb;
		}
		bitset& set()
		{
			base_type::do_set();
			return *this;
		}
		bitset& set(std::size_t pos)
		{
			if(pos >= Nb)
			{
				throw std::out_of_range("bitset<>: index out of range");
			}
			base_type::getword(pos) |= base_type::maskbit(pos);
			return *this;
		}
		bitset& set(std::size_t pos, word_type v)
		{
			if(pos >= Nb)
			{
				throw std::out_of_range("bitset<>: index out of range");
			}
			if(v)
			{
				base_type::getword(pos) |= base_type::maskbit(pos);
			}
			else
			{
				base_type::getword(pos) &= ~base_type::maskbit(pos);
			}
			return *this;
		}
		bitset& reset()
		{
			base_type::reset_impl();
			return *this;
		}
		bitset& flip()
		{
			base_type::flip_impl();
			return *this;
		}
		constexpr std::size_t count() const
		{
			return base_type::template count_impl<Nb>();
		}
		constexpr bool all() const
		{
			return base_type::template all_impl<Nb>();
		}
		constexpr bool any() const
		{
			return base_type::template any_impl<Nb>();
		}
		constexpr bool none() const
		{
			return !any();
		}
		constexpr bitset operator&(bitset const& r) const
		{
			return static_cast<bitset>(base_type::and_impl(r));
		}
		constexpr bitset operator|(bitset const& r) const
		{
			return static_cast<bitset>(base_type::or_impl(r));
		}
		constexpr bitset operator^(bitset const& r) const
		{
			return static_cast<bitset>(base_type::xor_impl(r));
		}
		constexpr bitset operator<<(size_type shift) const
		{
			return static_cast<bitset>(base_type::left_shift_impl(shift));
		}
		constexpr bitset operator>>(size_type shift) const
		{
			return static_cast<bitset>(base_type::right_shift_impl(shift));
		}
		bitset& operator&=(bitset const& r)
		{
			base_type::and_eq_impl(r);
			return *this;
		}
		bitset& operator|=(bitset const& r)
		{
			base_type::or_eq_impl(r);
			return *this;
		}
		bitset& operator^=(bitset const& r)
		{
			base_type::xor_eq_impl(r);
			return *this;
		}
		bitset& operator<<=(size_type shift)
		{
			base_type::left_shift_eq_impl(shift);
			return *this;
		}
		bitset& operator>>=(size_type shift)
		{
			base_type::right_shift_eq_impl(shift);
			return *this;
		}
		constexpr bitset operator~() const
		{
			return static_cast<bitset>(base_type::complement_impl());
		}
		constexpr bool operator==(bitset const& r) const
		{
			return base_type::template is_equal_impl<Nb>(r);
		}
		constexpr bool operator<(bitset const& r) const
		{
			return base_type::template is_less_impl<Nb>(r);
		}
		void swap(bitset& y) noexcept
		{
			base_type::swap(y);
		}
	};
}

#endif // RUIN_CONTAINER_BITSET_HPP_INCLUDED
