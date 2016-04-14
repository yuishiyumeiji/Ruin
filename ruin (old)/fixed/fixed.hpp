
#ifndef RUIN_FIXED_FIXED_HPP_INCLUDED
#define RUIN_FIXED_FIXED_HPP_INCLUDED

#include <cstddef>
#include <type_traits>

namespace ruin
{
	template<class Integral, std::size_t Bias>
	class fixed
	{
	public:
		constexpr fixed()
			: val_()
		{ }
		constexpr fixed(Integral const& v)
			: val_(v << Bias)
		{ }
		constexpr fixed(fixed const& r)
			: val_(r.val_)
		{ }
	public:
		constexpr fixed operator+(Integral const& r) const
		{
			return fixed(internal_construct_t(), val_ + r << Bias);
		}
		constexpr fixed operator+(fixed const& r) const
		{
			return fixed(internal_construct_t(), val_ + r.val_);
		}
		constexpr fixed operator-(Integral const& r) const
		{
			return fixed(internal_construct_t(), val_ - r << Bias);
		}
		constexpr fixed operator-(fixed const& r) const
		{
			return fixed(internal_construct_t(), val_ - r.val_);
		}
		constexpr fixed operator*(Integral const& r) const
		{
			return fixed(internal_construct_t(), val_ * r);
		}
		constexpr fixed operator*(fixed const& r) const
		{
			return fixed(internal_construct_t(), val_ * r.val_ >> Bias);
		}
		constexpr fixed operator/(Integral const& r) const
		{
			return fixed(internal_construct_t(), val_ / r);
		}
		constexpr fixed operator/(fixed const& r) const
		{
			return fixed(internal_construct_t(), (val_ << Bias) / r.val_);
		}
		constexpr Integral get_digits() const
		{
			return val_;
		}
	private:
		Integral val_;
	private:
		struct internal_construct_t
		{ };
		constexpr fixed(internal_construct_t, Integral const& v)
			: val_(v)
		{ }
	};
}

#endif // RUIN_FIXED_FIXED_HPP_INCLUDED

