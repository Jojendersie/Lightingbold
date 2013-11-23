#include "GameObject.hpp"
#include "../math/math.hpp"
#include <cstdlib>

namespace Ai
{
	GameObject::~GameObject()
	{
		free(m_direction);
		free(m_goal);
		free(m_position);
	}

	GameObject::GameObject()
	{
		m_direction = new Math::Vec2(0);
		m_energy = 0;
		m_goal = new Math::Vec2(0);
		m_position = new Math::Vec2(0);
		m_radius = 0;
	}

	void GameObject::setDirection(const Math::Vec2& _newDirection){
		m_direction = new Math::Vec2(_newDirection);
	}

	void GameObject::setEnergy(float _newEnergy){
		m_energy = _newEnergy;
	}

	void GameObject::setGoal(const Math::Vec2& _newGoal){
		m_goal = new Math::Vec2(_newGoal);
	}

	void GameObject::setPosition(const Math::Vec2& _newPosition){
		m_position = new Math::Vec2(_newPosition);
	}

	void GameObject::setRadius(float _newRadius){
		m_radius = _newRadius;
	}

	const Math::Vec2& GameObject::getPosition()
	{
		return *m_position;
	}

	const Math::Vec2& GameObject::getDiretion()
	{
		return Vec2(0);
	}

	const Math::Vec2& GameObject::getGoal()
	{
		return Vec2(0);
	}

	float GameObject::getRadius()
	{
		return m_radius;
	}

	float GameObject::getEnergy()
	{
		return 0;
	}

	float GameObject::getKineticEnegy()
	{
		return 0;
	}

	void GameObject::update()
	{
		
	}
}