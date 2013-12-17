#include "GameObject.hpp"
#include "../math/math.hpp"
#include <cstdlib>
#include "../map/map.hpp"
#include "Enemy.hpp"

namespace Ai
{
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
	}

	GameObject::GameObject(const Math::Vec2& _position, float _energy,Map::Map* _map) : m_map(_map), m_position(_position), m_energy(_energy), m_goal(_position)
	{
		m_direction = 0;

		setRadius();

		m_accelerationPerFrame = 0.015f;
		m_maxAcceleration = 0.0005f;
		m_friction = 0.975f;

		m_sprint = false;
		m_maxTime = 0.25f;
		m_time = 0.0f;
		m_alive = true;
		m_timeColliding = 0.0;
		m_minEnergy = 0.05;
	}

	void GameObject::setDirection(const Math::Vec2& _newDirection){
		m_direction = _newDirection;
	}

	void GameObject::setEnergy(float _newEnergy){
		m_energy = _newEnergy;
		if(m_energy < m_minEnergy)
			setAlive(false);
		setRadius();
	}

	void GameObject::setGoal(const Math::Vec2& _newGoal){
		m_goal = _newGoal;
	}

	void GameObject::setPosition(const Math::Vec2& _newPosition){
		m_position = _newPosition;
	}

	void GameObject::setRadius(){
		// TODO compute
		m_radius = sqrt( m_energy) *0.1f;
	}

	const Math::Vec2& GameObject::getPosition()
	{
		return m_position;
	}

	const Math::Vec2& GameObject::getDiretion()
	{
		return m_direction;
	}

	const Math::Vec2& GameObject::getGoal()
	{
		return m_goal;
	}

	float GameObject::getRadius()
	{
		return m_radius;
	}

	float GameObject::getEnergy()
	{
		return m_energy;
	}

	float GameObject::getKineticEnegy()
	{
		return m_direction.length();
	}

	void GameObject::update( double _deltaTime)
	{
		if(isAlive())
		{
			checkCollision(_deltaTime);
			movement(_deltaTime);
			checkForBoundaries();
		}
	}

	void GameObject::movement( double _deltaTime)
	{
		//m_direction = m_direction.normalize() * (m_direction.length() *0.9f);
		// compute current target and how to get there
		Math::Vec2 difference = m_goal - m_position;
		Math::Vec2 differenceNormal = difference.normalize();

        float distance = difference.length();

		// every step we throw in some friction, to slow the object down
		m_direction *= m_friction;

		Math::Vec2 acceleration = differenceNormal * m_accelerationPerFrame * _deltaTime;
        //acceleration = acceleration / (distance * distance);

		if(acceleration.length() > m_maxAcceleration) acceleration = acceleration.normalize() * m_maxAcceleration;

		//Math::Vec2 force = m_energy * acceleration;
		//force *= m_gravityFactor;
        
		// that is our new movement
		m_direction += acceleration;

		// and we wish to move ^^
		if(m_sprint)
		{
			m_position += m_direction;
			m_position += m_direction;
			//m_position += m_direction;
			if(m_time<0)
				m_sprint = false;
			else
				m_time -= _deltaTime;
		}
		else
		{
			m_position += m_direction;
		}
	}

	void GameObject::checkCollision(double _deltaTime)
	{
		if(m_map->getPlayer() != this && m_map->getPlayer()->isAlive())
		{
			if(getRadius() > m_map->getPlayer()->getRadius())
			{
				float percentageOverlap = g_circlePercentageOverlap(getPosition(),getRadius(),m_map->getPlayer()->getPosition(),m_map->getPlayer()->getRadius());
				if(percentageOverlap >= 0)
				{
					float enemyEnergy = m_map->getPlayer()->getEnergy();
					float stealEnergy = (enemyEnergy * percentageOverlap) * 0.1;

					setEnergy(getEnergy() + stealEnergy);
					m_map->getPlayer()->setEnergy(enemyEnergy-stealEnergy);
				}
			}
		}
		for(int i=0;i<m_map->getNumberOfObjects();++i)
		{
			if(m_map->getEnemy(i) != this && m_map->getEnemy(i)->isAlive())
			{
				if(getRadius() > m_map->getEnemy(i)->getRadius())
				{
					float percentageOverlap = g_circlePercentageOverlap(getPosition(),getRadius(),m_map->getEnemy(i)->getPosition(),m_map->getEnemy(i)->getRadius());
					if(percentageOverlap >= 0)
					{
						float enemyEnergy = m_map->getEnemy(i)->getEnergy();
						float stealEnergy = (enemyEnergy * percentageOverlap) * 0.1;

						setEnergy(getEnergy() + stealEnergy);
						m_map->getEnemy(i)->setEnergy(enemyEnergy-stealEnergy);
					}
				}
				//if(g_circleCollision(getPosition(),getRadius(),m_map->getEnemy(i)->getPosition(),m_map->getEnemy(i)->getRadius()))
				//{
				//	m_timeColliding+=_deltaTime;
				//	if(getRadius() > m_map->getEnemy(i)->getRadius())
				//	{
						// energy of enemy
				//		float enemyEnergy = m_map->getEnemy(i)->getEnergy();
				//		float stealEnergy = m_timeColliding * 100 * enemyEnergy;

				//		setEnergy(getEnergy() + stealEnergy);
				//		m_map->getEnemy(i)->setEnergy(enemyEnergy-stealEnergy);
						/*setEnergy(getEnergy()+m_map->getEnemy(i)->getEnergy());
						m_map->getEnemy(i)->setAlive(false);*/
						/*int nShape1 = m_map->getEnemy(i)->getShape1();
						int nShape2 = m_map->getEnemy(i)->getShape2();
						float nInterpolation = m_map->getEnemy(i)->getshapeInterpolation();
						m_shape1 = (int)floor(m_shape1 + (nShape1-m_shape1)*0.5f+0.5);
						m_shape2 = (int)floor(m_shape2 + (nShape2-m_shape2)*0.5f+0.5);
						m_shapeInterpolation = (int)floor(m_shapeInterpolation + (nInterpolation-m_shapeInterpolation)*0.5f+0.5);*/
				//	}
					/*else if(getRadius()<m_map->getEnemy(i)->getRadius())
					{
						//m_alive=false;
					}
					else if(getDiretion().length() >= m_map->getEnemy(i)->getDiretion().length())
					{
						setEnergy(getEnergy()+m_map->getEnemy(i)->getEnergy());
						m_map->getEnemy(i)->setAlive(false);
					}
					else
					{
						m_alive=false;
					}*/
				//}
				//else
				//{
				//	m_timeColliding = 0.0;
				//}
			}
		}
	}

	void GameObject::checkForBoundaries()
	{
		float reflectionSlowdown = 0.9;
		if (m_position.x > 1.0f)
		{
			m_position.x = 1;
			m_direction.x *= -reflectionSlowdown;
		}
		if(m_position.y > 1.0f)
		{
			m_position.y = 1;
			m_direction.y *= -reflectionSlowdown;
		}
		if (m_position.x < -1.0f)
		{
			m_position.x = -1;
			m_direction.x *= -reflectionSlowdown;
		}
		if(m_position.y < -1.0f)
		{
			m_position.y = -1;
			m_direction.y *= -reflectionSlowdown;
		}
	}

	float GameObject::getShinyRadius()
	{
		return getRadius()*2;
	}
}