#pragma once

#include "../predecl.hpp"
#include "../utilities/smalldynarray.hpp"

namespace Map {

	/// \brief Representation of the background potential field.
	class Map
	{
	public:

		/// \brief constructor
		Map(int _width, int _height);

		/// \brief destructor
		~Map();

		/// \brief Computes the position where the ray hits the scene
		Math::Vec2 RayCast( const Math::Vec2& _position, const Math::Vec2& _direction );

		/// \brief Computes the gradient of the map at the given position
		Math::Vec2 Gradient( const Math::Vec2& _position );

		/// \breif Returns the density at the given position
		float Density( const Math::Vec2& _position);

		/// \brief Updates the map
		void Update();

		void addEnemy(const Math::Vec2& _position, float _radius);

		/// \brief all objects without the player
		int getNumberOfObjects() {return m_objects.size(); }
		Ai::Enemy* getEnemy(int _idx) {return m_objects[_idx]; }

		Ai::GameObject* getPlayer() {return m_player;}
	private:
		int m_height;
		int m_width;
		float *m_densityMap;
		Utilities::DynArray<Ai::Enemy> m_objects;
		Ai::GameObject* m_player;

		// initialization
		void InitMap();

		// function to compute a gauss blob, you get the value for the given coordinates
		float GaussAtCoordinate(const Math::Vec2& _coordinate, const Math::Vec2& _center, const Math::Vec2& spread, float _amplidude);
	};

} // namespace Map