
#ifndef RUIN_VARIANT_BAD_VISIT_HPP_INCLUDED
#define RUIN_VARIANT_BAD_VISIT_HPP_INCLUDED

#include <stdexcept>

namespace ruin
{
	class bad_visit
		: public std::exception
	{
	public:
		virtual char const*
		what() const noexcept override
		{
			return "ruin::bad_visit";
		}
	};
}

#endif // RUIN_VARIANT_BAD_VISIT_HPP_INCLUDED

