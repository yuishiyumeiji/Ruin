//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_LAMBDA_LAMBDA_HPP_INCLUDED
#define RUIN_LAMBDA_LAMBDA_HPP_INCLUDED

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include "ruin/utility/index_tuple.hpp"

namespace ruin
{
	namespace lambda
	{
		namespace detail
		{
			template<class Tuple1, class Tuple2, std::size_t... Indices1, std::size_t... Indices2>
			constexpr auto append_impl(Tuple1 const& tuple1, Tuple2 const& tuple2, ruin::index_tuple<Indices1...>, ruin::index_tuple<Indices2...>)
				-> decltype(std::make_tuple(std::get<Indices1>(tuple1)..., std::get<Indices2>(tuple2)...))
			{
				return std::make_tuple(std::get<Indices1>(tuple1)..., std::get<Indices2>(tuple2)...);
			}
			template<class Tuple1, class Tuple2>
			constexpr auto append(Tuple1 const& tuple1, Tuple2 const& tuple2)
				-> decltype(ruin::lambda::detail::append_impl(tuple1, tuple2, ruin::index_range<0, std::tuple_size<Tuple1>::value>::make(), ruin::index_range<0, std::tuple_size<Tuple2>::value>::make()))
			{
				return ruin::lambda::detail::append_impl(tuple1, tuple2, ruin::index_range<0, std::tuple_size<Tuple1>::value>::make(), ruin::index_range<0, std::tuple_size<Tuple2>::value>::make());
			}
		}

		namespace detail
		{
			template<std::size_t BV, class LE>
			struct binder
			{
				LE le_;
				constexpr binder(LE const& le)
					: le_(le)
				{ }
			};

			template<std::size_t, class, int = 0>
			struct get_impl;
			template<std::size_t BV, class Head, class... Tail, int I>
			struct get_impl<BV, std::tuple<Head, Tail...>, I>
				: public ruin::lambda::detail::get_impl<BV, std::tuple<Tail...>, I + 1>
			{ };
			template<std::size_t BV, class LE, class... Tail, int I>
			struct get_impl<BV, std::tuple<ruin::lambda::detail::binder<BV, LE>, Tail...>, I>
				: public std::integral_constant<std::size_t, I>
			{ };
			template<std::size_t BV, int I>
			struct get_impl<BV, std::tuple<>, I>
				: public std::integral_constant<std::size_t, -1>
			{ };
			template<std::size_t BV, class EnvList>
			constexpr auto get(EnvList const& env)
				-> decltype(std::get<ruin::lambda::detail::get_impl<BV, EnvList>::value>(env).le_)
			{
				return std::get<ruin::lambda::detail::get_impl<BV, EnvList>::value>(env).le_;
			}
		}

		class lambda_base
		{ };
		template<class X>
		struct is_lambda
			: public std::is_base_of<ruin::lambda::lambda_base, X>
		{ };
		template<class>
		struct is_binder
			: public std::integral_constant<bool, false>
		{ };
		template<std::size_t BV, class LE>
		struct is_binder<ruin::lambda::detail::binder<BV, LE>>
			: public std::integral_constant<bool, true>
		{ };
	
		namespace exp
		{
			template<std::size_t>
			struct variable;
			template<class, class>
			struct closure;
			template<class, class>
			struct apply;
			template<std::size_t, class>
			struct abstract;
			template<class>
			struct constant;
			
			//          X.eval(env)       => (value of (the first) X in env).eval(env)
			// let[env].in[e].eval(env')  => e.eval(env, env')
			//   e[a1,...,an].eval(env)   => e.eval(env)(a1.eval(env),...,an.eval(env))
			//   lambda(X)[e].eval(env)   => lambda(Y)[let[env].in[e[X->Y]]]
			//    constant(v).eval(env)   => v
			//
			//        lambda(X)[e](v)     => let[X == v].in[e].eval(empty-env)
			
			template<std::size_t VID>
			struct variable
				: public ruin::lambda::lambda_base
			{
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
					-> decltype(ruin::lambda::detail::get<VID>(env).eval(env))
				{
					return ruin::lambda::detail::get<VID>(env).eval(env);
				}
				template<class Arg>
				constexpr apply<variable, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<variable, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class EnvList, class LE>
			struct closure
				: public ruin::lambda::lambda_base
			{
			private:
				EnvList env_;
				LE le_;
			public:
				constexpr closure(EnvList const& env, LE const& le)
					: env_(env), le_(le)
				{ }
			public:
				template<class EnvList2>
				constexpr auto eval(EnvList2 const& env2) const
					-> decltype(closure::le_.eval(ruin::lambda::detail::append(closure::env_, env2)))
				{
					return le_.eval(ruin::lambda::detail::append(env_, env2));
				}
				template<class Arg>
				constexpr apply<closure, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<closure, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class LE, class ArgList>
			struct apply
				: public ruin::lambda::lambda_base
			{
			private:
				LE le_;
				ArgList args_;
			public:
				constexpr apply(LE const& le, ArgList const& args)
					: le_(le), args_(args)
				{ }
			private:
				template<class EnvList, std::size_t... Indices>
				static constexpr auto eval_impl(LE const& le, ArgList const& args, EnvList const& env, ruin::index_tuple<Indices...>)
					-> decltype(le.eval(env)(std::get<Indices>(args).eval(env)...))
				{
					return le.eval(env)(std::get<Indices>(args).eval(env)...);
				}
			public:
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
					-> decltype(eval_impl(apply::le_, apply::args_, env, ruin::index_range<0, std::tuple_size<ArgList>::value>::make()))
				{
					return eval_impl(le_, args_, env, ruin::index_range<0, std::tuple_size<ArgList>::value>::make());
				}
				template<class Arg>
				constexpr apply<apply, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<apply, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<std::size_t BVID, class LE>
			struct abstract
				: public ruin::lambda::lambda_base
			{
			private:
				LE le_;
			public:
				constexpr abstract(LE const& le)
					: le_(le)
				{ }
			public:
				template<class Arg>
				constexpr auto operator()(Arg const& arg) const
					-> decltype(abstract::le_.eval(std::make_tuple(ruin::lambda::exp::variable<BVID>() == arg)))
				{
					return le_.eval(std::make_tuple(ruin::lambda::exp::variable<BVID>() == arg));
				}
				// substitution [X->X'] is unimplemented
				template<class EnvList>
				constexpr ruin::lambda::exp::abstract<BVID, closure<EnvList, LE>> eval(EnvList const& env) const
				{
					return {{env, le_}};
				}
				template<class Arg>
				constexpr apply<abstract, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<abstract, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class T>
			struct constant
				: public ruin::lambda::lambda_base
			{
			private:
				T value_;
			public:
				constexpr constant(T const& t)
					: value_(t)
				{ }
			public:
				template<class EnvList>
				constexpr T eval(EnvList const&) const
				{
					return value_;
				}
				template<class Arg>
				constexpr apply<constant, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<constant, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}
	
		template<class T>
		constexpr ruin::lambda::exp::constant<typename std::decay<T>::type> val(T t)
		{
			return {t};
		}
	
		namespace detail
		{
			template<class EnvList>
			struct let_impl1
			{
				EnvList const& env_;
				let_impl1 const& in;
				constexpr let_impl1(EnvList const& env)
					: env_(env), in(*this)
				{ }
				template<class LE>
				constexpr ruin::lambda::exp::closure<EnvList, LE> operator[](LE const& le) const
				{
					return {env_, le};
				}
			};
			struct let_impl2
			{
				template<class Binder>
				constexpr ruin::lambda::detail::let_impl1<std::tuple<Binder>> operator[](Binder const& bdr) const
				{
					return {std::make_tuple(bdr)};
				}
				template<class... Envs>
				constexpr ruin::lambda::detail::let_impl1<std::tuple<Envs...>> operator[](std::tuple<Envs...> const& bdrs) const
				{
					return {bdrs};
				}
			};
		}
		static constexpr ruin::lambda::detail::let_impl2 let{};
		
		namespace detail
		{
			template<std::size_t BVID>
			struct lambda_impl
			{
				template<class LE>
				constexpr ruin::lambda::exp::abstract<BVID, LE> operator[](LE const& le) const
				{
					return {le};
				}
			};
		}
		template<std::size_t BVID>
		constexpr ruin::lambda::detail::lambda_impl<BVID> lambda(ruin::lambda::exp::variable<BVID>)
		{
			return {};
		}
		
		namespace detail
		{
			template<class T, bool = ruin::lambda::is_lambda<T>::value>
			struct LEify
			{
				typedef T type;
				static constexpr T go(T const& x)
				{
					return x;
				}
			};
			template<class T>
			struct LEify<T, false>
			{
				typedef ruin::lambda::exp::constant<T> type;
				static constexpr type go(T const& x)
				{
					return {x};
				}
			};
		}
		template<class T>
		constexpr typename ruin::lambda::detail::LEify<T>::type leify(T const& t)
		{
			return ruin::lambda::detail::LEify<T>::go(t);
		}
		
		template<std::size_t BV, class T>
		constexpr ruin::lambda::detail::binder<BV, typename ruin::lambda::detail::LEify<T>::type> operator==(ruin::lambda::exp::variable<BV>, T t)
		{
			return {ruin::lambda::detail::LEify<T>::go(t)};
		}
		
		template<class T, class U, class = typename std::enable_if<(ruin::lambda::is_lambda<T>::value && ruin::lambda::is_lambda<U>::value) || (ruin::lambda::is_binder<T>::value && ruin::lambda::is_binder<U>::value)>::type>
		constexpr auto operator,(T const& t, U const& u)
			-> decltype(std::make_tuple(t, u))
		{
			return std::make_tuple(t, u);
		}
		template<class... List, class T, class = typename std::enable_if<ruin::lambda::is_lambda<T>::value || ruin::lambda::is_binder<T>::value>::type>
		constexpr auto operator,(std::tuple<List...> const& lis, T const& t)
			-> decltype(ruin::lambda::detail::append(lis, std::make_tuple(t)))
		{
			return ruin::lambda::detail::append(lis, std::make_tuple(t));
		}
	}
}

#endif // RUIN_LAMBDA_LAMBDA_HPP_INCLUDED

