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

		/// \breif Returns the hight of the map at the given position
		float High( const Math::Vec2& _position);
	private:
	};

} // namespace Map