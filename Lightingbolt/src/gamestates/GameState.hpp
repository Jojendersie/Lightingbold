#pragma once

#include "../predecl.hpp"

namespace GameStates {
	class IGameState
	{
	public:
		virtual ~IGameState()	{}
		virtual void MouseMove(int _dx, int _dy)	{}
		virtual void KeyDown(int _key)				{}
		virtual void KeyUp(int _key)				{}
		virtual void Scroll(int _delta)				{}

		/// \brief Do the state specific rendering.
		/// \param [in] _time Absoulte time since game start in seconds.
		/// \param [in] _deltaTime Time since last frame.
		/// \param [inout] _renderTargets List of all rendertargets. The state
		///		can use them as it likes.
		/// \param [in] _shaders The global maintained list of all shaders.
		virtual void Render( double _time, double _deltaTime,
			Graphic::RenderTargetList& _renderTargets,
			Graphic::ShaderList& _shaders ) = 0;

		virtual void Update( double _time, double _deltaTime ) = 0;
	};
} // namespace GameStates