//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_LAMBDA_OPERATOR_HPP_INCLUDED
#define RUIN_LAMBDA_OPERATOR_HPP_INCLUDED

#include "ruin/functional/arithmetic.hpp"
#include "ruin/functional/bitwise.hpp"
#include "ruin/functional/logic.hpp"
#include "ruin/lambda/lambda.hpp"

namespace ruin
{
	namespace lambda
	{
		namespace exp
		{
			template<class LE1, class LE2>
			constexpr auto operator+(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::plus_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::plus_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator-(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::minus_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::minus_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator*(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::multiply_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::multiply_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator/(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::divide_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::divide_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator&(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::and_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::and_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator|(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::or_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::or_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator^(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::xor_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::xor_t())[le1, le2];
			}
			template<class LE>
			constexpr auto operator~(LE const& le)
				-> decltype(ruin::lambda::leify(ruin::not_t())[le])
			{
				return ruin::lambda::leify(ruin::not_t())[le];
			}
			template<class LE1, class LE2>
			constexpr auto operator&&(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::land_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::land_t())[le1, le2];
			}
			template<class LE1, class LE2>
			constexpr auto operator||(LE1 const& le1, LE2 const& le2)
				-> decltype(ruin::lambda::leify(ruin::lor_t())[le1, le2])
			{
				return ruin::lambda::leify(ruin::lor_t())[le1, le2];
			}
			template<class LE>
			constexpr auto operator!(LE const& le)
				-> decltype(ruin::lambda::leify(ruin::lnot_t())[le])
			{
				return ruin::lambda::leify(ruin::lnot_t())[le];
			}
		}
	}
}

#endif // RUIN_LAMBDA_OPERATOR_HPP_INCLUDED

