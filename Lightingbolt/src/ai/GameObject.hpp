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
		virtual void update( double _deltaTime);
		float getShinyRadius();
		bool isAlive() {return m_alive;}
		void setShape(int _index1,int _index2,float _interpolation) { m_shape1=_index1;m_shape2=_index2;m_shapeInterpolation=_interpolation; }
		int getShape1() {return m_shape1; }
		int getShape2() {return m_shape2; }
		float getshapeInterpolation() {return m_shapeInterpolation; }
		void setMaterialIndex(int _index) {m_materialIndex = _index; }
		int getMaterialIndex() {return m_materialIndex; }
	private:
		void checkForBoundaries();
		void movement( double _deltaTime);
		void checkCollision();
		Math::Vec2 m_position;
		Math::Vec2 m_direction;
		Math::Vec2 m_goal;
		float m_radius;
		float m_energy;
		int m_shape1;
		int m_shape2;
		float m_shapeInterpolation;
		int m_materialIndex;

		float m_maxAcceleration;
		float m_friction;
		float m_accelerationPerFrame;
		
		bool m_alive;
	protected:
		Map::Map* m_map;
		void setAlive(bool _alive) {m_alive=_alive;}
	};

} // namespace gameObject