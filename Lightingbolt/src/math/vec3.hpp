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
	};
}