#pragma once

namespace Math {
	class Vec3
	{
	public:
		float x, y, z;

		/// \brief Create with a scalar value in all channels
		Vec3(float f) : x(f), y(f), z(f)	{}

		/// \brief Set all three components
		Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)	{}
	
		Vec3& operator=(const Vec3& _v)			{ x=_v.x; y=_v.y; z=_v.z; }

		Vec3 operator+(const Vec3& _v) const	{ return Vec3(x+_v.x, y+_v.y, z+_v.z); }
		Vec3 operator-(const Vec3& _v) const	{ return Vec3(x-_v.x, y-_v.y, z-_v.z); }
		Vec3 operator*(float _f) const			{ return Vec3(x*_f, y*_f, z*_f); }
		Vec3 operator/(float _f) const			{ return Vec3(x/_f, y/_f, z/_f); }
		/// \brief Component wise multiplication
		Vec3 operator*(const Vec3& _v) const	{ return Vec3(x*_v.x, y*_v.y, z*_v.z); }
		/// \brief Component wise division
		Vec3 operator/(const Vec3& _v) const	{ return Vec3(x/_v.x, y/_v.y, z/_v.z); }

		Vec3& operator+=(const Vec3& _v)		{ x+=_v.x, y+=_v.y; z+=_v.z; return *this; }
		Vec3& operator-=(const Vec3& _v)		{ x-=_v.x, y-=_v.y; z-=_v.z; return *this; }
		Vec3& operator*=(float _f)				{ x*=_f, y*=_f; z*=_f; return *this; }
		Vec3& operator*=(const Vec3& _v)		{ x*=_v.x, y*=_v.y; z*=_v.z; return *this; }
		Vec3& operator/=(float _f)				{ x/=_f, y/=_f; z/=_f; return *this; }
		Vec3& operator/=(const Vec3& _v)		{ x/=_v.x, y/=_v.y; z/=_v.z; return *this; }

		bool operator==(const Vec3& _v)			{ return (_v-*this).lengthSq() < 0.00001f; }
		bool operator!=(const Vec3& _v)			{ return (_v-*this).lengthSq() >= 0.00001f; }

		/// \brief Eclidean norm
		float length() const					{ return sqrt(x*x + y*y + z*z); }

		/// \brief Eclidean norm squared: x^2+y^2
		float lengthSq() const					{ return x*x + y*y + z*z; }

		Vec3 cross(const Vec3& _v) const		{ return Vec3(y * _v.z - z * _v.y, z * _v.x - x * _v.z, x * _v.y - y * _v.x);}

		float dot(const Vec3& _v) const			{ return x*_v.x + y*_v.y + z*_v.z; }
	};

	Vec3 operator*(float _f, const Vec3& _v)	{ return Vec3(_v.x*_f, _v.y*_f, _v.z*_f); }

	/// \brief Linear interpolation
	inline Vec3	lerp(const Vec3& v1, const Vec3& v2, const float f)	{return v1 + (v2-v1)*f;}
}