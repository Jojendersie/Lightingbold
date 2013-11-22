#pragma once

#include "../predecl.hpp"

namespace Map {

	/// \brief Representation of the background potential field.
	class Map
	{
	public:
		/// \brief Computes the position where the ray hits the scene
		Math::Vec2 RayCast( const Math::Vec2& _position, const Math::Vec2& _direction );

		/// \brief Computes the gradient of the map at the given position
		Math::Vec2 Gradient( const Math::Vec2& _position );

		/// \breif Returns the density at the given position
		float Density( const Math::Vec2& _position);

		/// \brief Updates the map
		void Update();
	private:
		int m_height;
		int m_width;
		float *m_densityMap;
	};

} // namespace Map