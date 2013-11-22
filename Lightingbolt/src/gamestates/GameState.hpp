#pragma once

#include "../predecl.hpp"

namespace GameStates {
	class IGameState
	{
	public:
		virtual void MouseMove(int _dx, int _dy)	{}
		virtual void KeyDown(int _key)				{}
		virtual void KeyUp(int _key)				{}
		virtual void Scroll(int _delta)				{}

		/// \brief Do the state specific rendering.
		/// \param [in] _time Absoulte time since game start in seconds.
		/// \param [in] _deltaTime Time since last frame.
		/// \param [inout] _renderTargets List of all rendertargets. The state
		///		can use them as it likes.
		virtual void Render( double _time, double _deltaTime,
			Graphic::RenderTargetList& _renderTargets ) = 0;
	};
} // namespace GameStates