#include "Enemy.hpp"
#include "../map/map.hpp"
#include "../math/math.hpp"


namespace Ai
{
	void Enemy::think()
	{
		
		for(int i = 0;i < m_map->getNumberOfObjects(); i++)
		{
			Enemy* currentEnemy = m_map->getEnemy(i);
			if(this!=currentEnemy)
			{
				if(g_circleCollision(currentEnemy->getPosition(),currentEnemy->getShinyRadius(),getPosition(),getShinyRadius()))
				{
					setGoal(currentEnemy->getPosition());
					break;
				}
			}
		}
	}

	Enemy::Enemy()
	{
		
	}

	Enemy::Enemy(const Math::Vec2& _position, float _radius, Map::Map* _map): GameObject(_map)
	{
		setPosition(_position);
		setRadius(_radius);
	}

	Enemy::~Enemy()
	{

	}

	void Enemy::update()
	{
		//think();
		GameObject::update();
	}
} // namespace Ai