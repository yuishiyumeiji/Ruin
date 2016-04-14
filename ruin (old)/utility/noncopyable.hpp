
#ifndef RUIN_UTILITY_NONCOPYABLE_HPP_INCLUDED
#define RUIN_UTILITY_NONCOPYABLE_HPP_INCLUDED

namespace ruin
{
	namespace noncopyable_
	{
		class noncopyable
		{
		protected:
			noncopyable() = default;
			noncopyable(noncopyable const&) = delete;
			void
			operator=(noncopyable const&) = delete;
			noncopyable(noncopyable&&) = default;
			noncopyable&
			operator=(noncopyable&&) = default;
		};
	} // namespace noncopyable_

	using ruin::noncopyable_::noncopyable;
}

#endif // RUIN_UTILITY_NONCOPYABLE_HPP_INCLUDED
