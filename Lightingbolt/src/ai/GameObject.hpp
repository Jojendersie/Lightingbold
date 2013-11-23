#pragma once

#include "../predecl.hpp"

using namespace Math;

namespace Ai {

	class GameObject{
	public:
		GameObject();
		~GameObject();
		const Math::Vec2& getPosition();
		void setPosition(const Math::Vec2& _newPosition);
		const Math::Vec2& getDiretion();
		void setDirection(const Math::Vec2& _newDirection);
		const Math::Vec2& getGoal();
		void setGoal(const Math::Vec2& _newGoal);
		float getRadius();
		void setRadius(float _newRadius);
		float getEnergy();
		void setEnergy(float _newEnergy);
		float getKineticEnegy();
		void update();
	private:
		Math::Vec2 *m_position;
		Math::Vec2 *m_direction;
		Math::Vec2 *m_goal;
		float m_radius;
		float m_energy;
	};

} // namespace gameObject