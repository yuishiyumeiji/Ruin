
#ifndef RUIN_TUPLE_HPP_INCLUDED
#define RUIN_TUPLE_HPP_INCLUDED

#include "ruin/tuple/array.hpp"
#include "ruin/tuple/c_array.hpp"
#include "ruin/tuple/valarray.hpp"
#include "ruin/tuple/tuple.hpp"
#include "ruin/tuple/tuple_element.hpp"
#include "ruin/tuple/tuple_get.hpp"
#include "ruin/tuple/tuple_size.hpp"

namespace ruin
{
	using ruin::tuples::tuple;

	using ruin::tuples::tuple_element;
	using ruin::tuples::tuple_get;
	using ruin::tuples::tuple_size;

	using ruin::tuples::forward_as_tuple;
	using ruin::tuples::make_tuple;
	using ruin::tuples::make_tie;
}

#endif // RUIN_TUPLE_HPP_INCLUDED
