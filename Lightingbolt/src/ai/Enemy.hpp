#pragma once

#include "../predecl.hpp"
#include "GameObject.hpp"

namespace Ai
{
	class Enemy : GameObject
	{
	public:
		Enemy();
		Enemy(const Math::Vec2& _position, float _radius);
		~Enemy();
	private:

	};
}