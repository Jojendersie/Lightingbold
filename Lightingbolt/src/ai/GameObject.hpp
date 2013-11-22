#pragma once

#include "../predecl.hpp"
#include "../math/math.hpp"

using namespace Math;

namespace ai {

class GameObject{
public:
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
    void update();
private:
    Vec2 m_position;
    Vec2 m_direction;
    Vec2 m_goal;
    float m_radius;
    float m_energy;
};

} // namespace gameObject