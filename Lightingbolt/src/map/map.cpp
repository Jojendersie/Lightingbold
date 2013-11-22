#include "map.hpp"

namespace Map
{
	Map::Map(int _width, int _height)
	{
		m_width = _width;
		m_height = _height;

		m_densityMap = new float[_width*_height];

		initMap();
	}

	Map::~Map()
	{
		free(m_densityMap);
		free(m_player);
		free(m_objects);
	}

	void Map::initMap()
	{
		// build density map
	}

} // namespace Map
