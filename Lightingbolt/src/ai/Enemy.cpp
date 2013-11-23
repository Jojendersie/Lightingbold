#include "Enemy.hpp"
#include "../map/map.hpp"
#include "../math/math.hpp"
#include "../generator/Random.hpp"
#include <cmath>

extern Generators::Random* g_rand;

namespace Ai
{
	Math::Vec2 Enemy::determineReaction(GameObject* _currentEnemy)
	{
		float vectorWeight = 1;
		Math::Vec2 directionVector(_currentEnemy->getPosition()-getPosition());
		directionVector = directionVector / directionVector.length();
		float sizeRatio;
		sizeRatio = _currentEnemy->getEnergy()/getEnergy();
		if(sizeRatio > 1){
			vectorWeight *= -1;
		}
		return vectorWeight *directionVector;
	}

	void Enemy::think()
	{
		GameObject* bestEnemy = nullptr;
		Math::Vec2 newGoal(0);
		if(g_circleCollision(m_map->getPlayer()->getPosition(),m_map->getPlayer()->getShinyRadius(),getPosition(),getShinyRadius()))
		{
			newGoal += determineReaction(m_map->getPlayer());
		}
		for(int i = 0;i < m_map->getNumberOfObjects(); i++)
		{
			Enemy* currentEnemy = m_map->getEnemy(i);

			if(this!=currentEnemy)
			{
				if(g_circleCollision(currentEnemy->getPosition(),currentEnemy->getShinyRadius(),getPosition(),getShinyRadius()))
				{
					newGoal += determineReaction(currentEnemy);
				}
			}
		}
		if(newGoal.length()==0){
			newGoal = Math::Vec2(g_rand->Uniform(-0.1f,0.1f),g_rand->Uniform(-0.1f,0.1f));
		}
		setGoal(getPosition()+newGoal);
	}

	Enemy::Enemy()
	{
		
	}

	Enemy::Enemy(const Math::Vec2& _position, float _energy, Map::Map* _map): GameObject(_map)
	{
		setPosition(_position);
		setEnergy(_energy);
	}

	Enemy::~Enemy()
	{

	}

	void Enemy::update()
	{
		think();
		GameObject::update();
	}
} // namespace Ai