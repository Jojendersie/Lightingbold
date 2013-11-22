#pragma once

#include "../predecl.hpp"
#include "../math/math.hpp"

using namespace Math;

namespace GameObject {

class GameObject{
public:
	Vec2 GetPosition();
    void SetPosition(Vec2 newPosition);
    Vec2 GetDiretion();
    void SetDirection(Vec2 newDirection);
    Vec2 GetGoal();
    void SetGoal(Vec2 newGoal);
    float GetRadius();
    void SetRadius(float newRadius);
    float GetEnergy();
    void SetEnergy(float newEnergy);
    float GetKineticEnegy();
    void update();
private:
    Vec2 position;
    Vec2 direction;
    Vec2 goal;
    float radius;
    float energy;
};

} // namespace gameObject