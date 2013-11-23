#include "map.hpp"
#include <cstdlib>
#include "../math/math.hpp"
#include "../ai/GameObject.hpp"
#include "../ai/Enemy.hpp"
#include <cmath>

namespace Map
{
	Map::Map(int _width, int _height)
	{
		m_width = _width;
		m_height = _height;

		m_densityMap = new float[_width*_height];
		
		m_player = new Ai::GameObject();
		InitMap();
	}

	Map::~Map()
	{
		free(m_densityMap);
		free(m_player);
	}

	void Map::InitMap()
	{
		// build density map
		for(int y=0;y<m_height;++y)
		{
			for(int x=0;x<m_width;++x)
			{
				m_densityMap[m_width*y+x]  = GaussAtCoordinate(Math::Vec2(x,y),Math::Vec2(m_width/2,m_height/2),Math::Vec2(m_width/4,m_height/4),1);
			} // for x
		} // for y
	}

	float Map::GaussAtCoordinate(const Math::Vec2& _coordinate, const Math::Vec2& _center, const Math::Vec2& spread, float _amplidude)
	{
		Math::Vec2 sub = _coordinate - _center;
		return _amplidude * pow(2.71828f,-(((sub.x*sub.x)/(2*spread.x*spread.x))+((sub.y*sub.y)/(2*spread.y*spread.y))));
	}

	void Map::Update()
	{
		// update the player
		m_player->update();

		// update all other objects
		for(int i=0;i<m_objects.size();++i)
		{
			m_objects[i]->update();
		}
		//m_objects
	}

	Math::Vec2 Map::RayCast( const Math::Vec2& _position, const Math::Vec2& _direction )
	{
		return Math::Vec2(0);
	}

		
	Math::Vec2 Map::Gradient( const Math::Vec2& _position )
	{
		return Math::Vec2(0);
	}

		
	float Map::Density( const Math::Vec2& _position)
	{
		return m_densityMap[m_width*(int)_position.y+(int)_position.x];
	}

	void Map::addEnemy(const Math::Vec2& _position, float _radius)
	{
		m_objects.append(Ai::Enemy(_position, _radius));
	}

} // namespace Map
