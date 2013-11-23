#include "Enemy.hpp"



namespace Ai
{
	Enemy::Enemy()
	{
		
	}

	Enemy::Enemy(const Math::Vec2& _position, float _radius)
	{
		setPosition(_position);
		setRadius(_radius);
	}

	Enemy::~Enemy()
	{
	}

} // namespace Ai