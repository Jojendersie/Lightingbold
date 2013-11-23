#include "GameObject.hpp"
#include "../math/math.hpp"

namespace Ai
{
	GameObject::~GameObject()
	{
	}

	GameObject::GameObject()
	{
		m_direction = Math::Vec2(0);
		m_energy = 0;
		m_goal = Math::Vec2(0);
		m_position = Math::Vec2(0);
		m_radius = 0;
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

	void GameObject::update()
	{
		Math::Vec2 targetDirection = m_goal - m_position;
		m_direction = targetDirection/targetDirection.length();// Math::lerp(m_direction,(targetDirection/targetDirection.length()), 1);

		m_position += m_direction;
	}
}