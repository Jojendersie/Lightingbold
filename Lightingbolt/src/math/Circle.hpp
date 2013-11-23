# pragma once
#include "vec2.hpp"

namespace Math{
	inline bool g_circleCollision(Vec2 _positionCircle1, float _radiusCircle1, Vec2 _positionCircle2, float _radiusCircle2){
		return ((_positionCircle1-_positionCircle2).length() <= (_radiusCircle1+_radiusCircle2));
	}
}