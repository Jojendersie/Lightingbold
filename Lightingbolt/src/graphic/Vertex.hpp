#pragma once

#include "../predecl.hpp"
#include "../math/math.hpp"

namespace Graphic {

	/// \brief A vertex which defines a quad and which has some extra parameters.
	struct Vertex
	{
		Math::Vec2 Position;	///< Screen space position
		Math::Vec2 Size;		///< Screen space size
		Math::Vec2 Rotation;	///< (sin(alpha), cos(alpha)) for rotation of the quad in 2D
		Math::Vec2 Param;		///< Can be used for shape parameters... (usage depends on shader)

		/// \brief Create the vertex layout for this vertex. Call this onth
		///		after a device was created.
		/// \details At the end call ReleaseLayout() before device destructoin.
		static void InitLayout(Shader* _shader);

		/// \brief Delete the one vertex layout.
		static void ReleaseLayout();

		/// \brief Set the vertex layout created by InitLayout().
		static void SetLayout();
	};

} // namespace Graphic