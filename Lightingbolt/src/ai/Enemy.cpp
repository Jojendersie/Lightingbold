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
		float optimalSizeRation = 0.75;
		Math::Vec2 directionVector(_currentEnemy->getPosition()-getPosition());
		directionVector = directionVector / directionVector.length();
		float sizeRatio;
		sizeRatio = _currentEnemy->getEnergy()/getEnergy();
		if(sizeRatio > 1){
			vectorWeight *= -1 * vectorWeight*vectorWeight*5.0f;
		}else
		{
			float sizeRatioRatioDiff = abs(optimalSizeRation-_currentEnemy->getRadius()/getRadius());
			vectorWeight *= (1/(sizeRatioRatioDiff*sizeRatioRatioDiff));
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
		m_currentThinkSleep = 0.0f;
		m_maxThinkSleep = 2.0f;
	}

	Enemy::Enemy(const Math::Vec2& _position, float _energy, Map::Map* _map): GameObject(_position,_energy,_map)
	{
		m_currentThinkSleep = 0.0f;
		m_maxThinkSleep = 1.0f;
	}

	Enemy::~Enemy()
	{

	}

	void Enemy::resetThinkSleep()
	{
		m_currentThinkSleep=0.0;
		m_maxThinkSleep = 0.25f + (double)g_rand->Uniform(0.0f,0.125f);
	}

	void Enemy::update( double _deltaTime)
	{
		if(m_currentThinkSleep > m_maxThinkSleep)
		{
			resetThinkSleep();
			think();
		}
		m_currentThinkSleep+=_deltaTime;
		GameObject::update(_deltaTime);
	}
} // namespace Ai