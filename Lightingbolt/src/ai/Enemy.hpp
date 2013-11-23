#pragma once

#include "../predecl.hpp"
#include "GameObject.hpp"

namespace Ai
{
	class Enemy : public GameObject
	{
	public:
		Enemy();
		Enemy(const Math::Vec2& _position, float _energy,Map::Map* _map);
		~Enemy();
		void think();
		Math::Vec2 Enemy::determineReaction(GameObject* _currentEnemy);
		virtual void update();
	private:
	};
}