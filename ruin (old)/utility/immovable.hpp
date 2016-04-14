
#ifndef RUIN_UTILITY_IMMOVABLE_HPP_INCLUDED
#define RUIN_UTILITY_IMMOVABLE_HPP_INCLUDED

namespace ruin
{
	namespace immovable_
	{
		class immovable
		{
		protected:
			immovable() = default;
			immovable(immovable const&) = delete;
			void
			operator=(immovable const&) = delete;
			immovable(immovable&&) = delete;
			void
			operator=(immovable&&) = delete;
		};
	} // namespace immovable_

	using ruin::immovable_::immovable;
}

#endif // RUIN_UTILITY_IMMOVABLE_HPP_INCLUDED
