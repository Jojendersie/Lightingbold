#pragma once

#include "../predecl.hpp"

namespace Graphic {

	/// \brief A double buffered vertex buffer for transform feedback.
	class FeedBackBuffer
	{
	public:
		FeedBackBuffer();

		/// \brief Toggles the two buffers and set one of them as target.
		void toggle();

		/// \brief Disable recording of geometry output
		void disable();

		/// \brief Draw the source buffer.
		void draw();
	private:
	};

} // namespace Graphic