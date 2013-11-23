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

	GameObject::GameObject(Map::Map* _map) : m_map(_map)
	{
		m_direction = Math::Vec2(0);
		//m_energy = 0.5f;
		m_goal = Math::Vec2(0);
		m_position = Math::Vec2(0);
		m_radius = 0;

		m_timetoReachMaxSpeed = 960.0f;
		m_maxSpeed = 0.04f;
		m_friction = 5/m_timetoReachMaxSpeed;
		m_acceleration = m_maxSpeed*m_friction;
		m_velocity = 0.0f;
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
		//m_velocity += m_acceleration - m_friction*m_velocity;
		//if(m_velocity.x > m_maxSpeed) m_velocity.x = m_maxSpeed;
		//if(m_velocity.y > m_maxSpeed) m_velocity.y = m_maxSpeed;

		Math::Vec2 targetDirection = m_goal - m_position;
		if(targetDirection.length() <= 0.005)
		{
			m_direction *= 0.6f;
			m_position += m_direction;
			return;
		}

		m_direction+=  /*targetDirection/targetDirection.length() *m_velocity;// * m_map->Density(m_position);*/ Math::lerp((m_direction),(targetDirection/targetDirection.length()*0.8), 0.00005f);

		//m_direction = m_direction/m_direction.length() * 0.0009;
		//m_velocity *= 0.5;

		m_direction *= 0.5;
		
		m_position += m_direction;

		//m_position = Math::lerp(m_position,(m_goal/*targetDirection.length()*/), 0.005);
	}

	float GameObject::getShinyRadius()
	{
		return getRadius()*2;
	}
}