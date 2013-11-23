#pragma once

#include "../predecl.hpp"
#include "../math/math.hpp"

using namespace Math;

namespace Ai {

class GameObject{
public:
	GameObject(Vec2 _position,Vec2 _direction,Vec2 _goal,float _radius,float _energy,int _shapeIndex): m_position(_position), m_direction(_direction), m_goal(_goal),m_radius(_radius),m_energy(_energy),m_shapeIndex(_shapeIndex){}
	virtual ~GameObject(){}
	Vec2 getPosition();
    void setPosition(Vec2 _newPosition);
    Vec2 getDiretion();
    void setDirection(Vec2 _newDirection);
    Vec2 getGoal();
    void setGoal(Vec2 _newGoal);
    float getRadius();
    void setRadius(float _newRadius);
    float getEnergy();
    void setEnergy(float _newEnergy);
    float getKineticEnegy();
	float getShinyRadius();
    void update();
	int getShapeIndex();
	void setShapeIndex(int _shapeIndex);
private:
    Vec2 m_position;
    Vec2 m_direction;
    Vec2 m_goal;
    float m_radius;
    float m_energy;
	int m_shapeIndex;
};

} // namespace gameObject