#pragma once

#include "../predecl.hpp"

namespace Graphic {

	/// \brief A double buffered vertex buffer for transform feedback.
	class FeedBackBuffer
	{
	public:
		FeedBackBuffer();
		~FeedBackBuffer();

		/// \brief Set target for transform feedback
		void enable();

		/// \brief Toggles the two buffers and set one of them as target.
		///		The other is used as current input vertex buffer.
		void toggle();

		/// \brief Disable recording of geometry output
		void disable();

		/// \brief Draw the source buffer.
		void draw();
	private:
		ID3D11Buffer* m_buffers[2];
		int m_source;					///< Which of the two buffers is the current source
	};

} // namespace Graphic