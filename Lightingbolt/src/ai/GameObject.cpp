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

	GameObject::GameObject(const Math::Vec2& _position, float _energy,Map::Map* _map) : m_map(_map), m_position(_position), m_energy(_energy)
	{
		m_direction = Math::Vec2(0);
		m_energy = 1.0f;
		m_goal = Math::Vec2(0);
		m_position = Math::Vec2(0);
		m_radius = 0;

		m_accelerationPerFrame = 0.00015f;
		m_maxAcceleration = 0.0005f;
		m_friction = 0.975f;
	}

	void GameObject::setDirection(const Math::Vec2& _newDirection){
		m_direction = Math::Vec2(_newDirection);
	}

	void GameObject::setEnergy(float _newEnergy){
		m_energy = _newEnergy;
	}

	void GameObject::setGoal(const Math::Vec2& _newGoal){
		m_goal = Math::Vec2(_newGoal);
	}

	void GameObject::setPosition(const Math::Vec2& _newPosition){
		m_position = Math::Vec2(_newPosition);
	}

	void GameObject::setRadius(float _newRadius){
		m_radius = _newRadius;
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
		return m_energy;
	}

	float GameObject::getEnergy()
	{
		return m_energy;
	}

	float GameObject::getKineticEnegy()
	{
		return m_direction.length();
	}

	void GameObject::update()
	{
		movement();
		checkForBoundaries();
	}

	void GameObject::movement()
	{
		//m_direction = m_direction.normalize() * (m_direction.length() *0.9f);
		// compute current target and how to get there
		Math::Vec2 difference = m_goal - m_position;
		Math::Vec2 differenceNormal = difference.normalize();

        float distance = difference.length();

		// every step we throw in some friction, to slow the object down
		m_direction *= 0.97f;//m_friction;

		Math::Vec2 acceleration = differenceNormal * 0.00015f;
        //acceleration = acceleration / (distance * distance);

		if(acceleration.length() > m_maxAcceleration) acceleration = acceleration.normalize() * m_maxAcceleration;

		//Math::Vec2 force = m_energy * acceleration;
		//force *= m_gravityFactor;
        
		// that is our new movement
		m_direction += acceleration;

		// end we wish to move ^^
		m_position += m_direction;
	}

	void GameObject::checkForBoundaries()
	{
		if (m_position.x > 1.0f)
		{
			m_position.x = 1;
		}
		if(m_position.y > 1.0f)
		{
			m_position.y = 1;
		}
		if (m_position.x < -1.0f)
		{
			m_position.x = -1;
		}
		if(m_position.y < -1.0f)
		{
			m_position.y = -1;
		}
	}

	float GameObject::getShinyRadius()
	{
		return getRadius()*2;
	}
}