#pragma once

#include "../predecl.hpp"
#include "../math/math.hpp"

using namespace Math;

namespace Ai {

	class GameObject{
	public:
		GameObject(const Math::Vec2& _position, float _energy,Map::Map* _map);
		GameObject();
		~GameObject();
		const Math::Vec2& getPosition();
		void setPosition(const Math::Vec2& _newPosition);
		const Math::Vec2& getDiretion();
		void setDirection(const Math::Vec2& _newDirection);
		const Math::Vec2& getGoal();
		void setGoal(const Math::Vec2& _newGoal);
		float getRadius();
		void setRadius();
		float getEnergy();
		void setEnergy(float _newEnergy);
		float getKineticEnegy();
		virtual void update();
		float getShinyRadius();
	private:
		void checkForBoundaries();
		void movement();
		Math::Vec2 m_position;
		Math::Vec2 m_direction;
		Math::Vec2 m_goal;
		float m_radius;
		float m_energy;

		float m_maxAcceleration;
		float m_friction;
		float m_accelerationPerFrame;
	protected:
		Map::Map* m_map;
	};

} // namespace gameObject