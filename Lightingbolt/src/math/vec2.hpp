#pragma once

#include <cmath>

namespace Math {

	class Vec2
	{
	public:
		float x, y;

		/// \brief Create with a scalar value in all channels
		Vec2(float f) : x(f), y(f)	{}

		/// \brief Set all two components
		Vec2(float _x, float _y) : x(_x), y(_y)	{}

		Vec2& operator=(const Vec2& _v)			{ x=_v.x; y=_v.y; }

		Vec2 operator+(const Vec2& _v) const	{ return Vec2(x+_v.x, y+_v.y); }
		Vec2 operator-(const Vec2& _v) const	{ return Vec2(x-_v.x, y-_v.y); }
		Vec2 operator*(float _f) const			{ return Vec2(x*_f, y*_f); }
		Vec2 operator/(float _f) const			{ return Vec2(x/_f, y/_f); }
		/// \brief Component wise multiplication
		Vec2 operator*(const Vec2& _v) const	{ return Vec2(x*_v.x, y*_v.y); }
		/// \brief Component wise division
		Vec2 operator/(const Vec2& _v) const	{ return Vec2(x/_v.x, y/_v.y); }

		Vec2& operator+=(const Vec2& _v)		{ x+=_v.x, y+=_v.y; return *this; }
		Vec2& operator-=(const Vec2& _v)		{ x-=_v.x, y-=_v.y; return *this; }
		Vec2& operator*=(float _f)				{ x*=_f, y*=_f; return *this; }
		Vec2& operator*=(const Vec2& _v)		{ x*=_v.x, y*=_v.y; return *this; }
		Vec2& operator/=(float _f)				{ x/=_f, y/=_f; return *this; }
		Vec2& operator/=(const Vec2& _v)		{ x/=_v.x, y/=_v.y; return *this; }

		bool operator==(const Vec2& _v)			{ return (_v-*this).lengthSq() < 0.00001f; }
		bool operator!=(const Vec2& _v)			{ return (_v-*this).lengthSq() >= 0.00001f; }

		/// \brief Eclidean norm
		float length() const					{ return sqrt(x*x + y*y); }

		/// \brief Eclidean norm squared: x^2+y^2
		float lengthSq() const					{ return x*x + y*y; }

		Vec2 perpendicular() const				{ return Vec2(-x, y);}

		/// \brief Reflect this vector at the given normal.
		Vec2 reflect(const Vec2& _n) const		{ return _n * (2.0f * dot(_n)) - (*this); }

		float dot(const Vec2& _v) const			{ return x*_v.x + y*_v.y; }
	};

	inline Vec2 operator*(float _f, const Vec2& _v)	{ return Vec2(_v.x*_f, _v.y*_f); }

	/// \brief Linear interpolation
	inline Vec2	lerp(const Vec2& v1, const Vec2& v2, const float f)	{return v1 + (v2-v1)*f;}
}