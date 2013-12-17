# pragma once
#include "vec2.hpp"

namespace Math{
	inline bool g_circleCollision(Vec2 _positionCircle1, float _radiusCircle1, Vec2 _positionCircle2, float _radiusCircle2){
		return ((_positionCircle1-_positionCircle2).length() <= (_radiusCircle1+_radiusCircle2));
	}

	inline float g_circleOverlap(Vec2 _positionCircle1, float _radiusCircle1, Vec2 _positionCircle2, float _radiusCircle2){
		if(g_circleCollision(_positionCircle1, _radiusCircle1, _positionCircle2, _radiusCircle2))
			return abs((_positionCircle1-_positionCircle2).length() - (_radiusCircle1+_radiusCircle2));
		else return -1;
	}

	inline float g_circlePercentageOverlap(Vec2 _positionCircle1, float _radiusCircle1, Vec2 _positionCircle2, float _radiusCircle2){
		if(g_circleCollision(_positionCircle1, _radiusCircle1, _positionCircle2, _radiusCircle2))
			return abs((_positionCircle1-_positionCircle2).length() - (_radiusCircle1+_radiusCircle2))/_radiusCircle1;
		else return -1.0f;
	}
}