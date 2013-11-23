#pragma once

#include "GameState.hpp"

namespace GameStates {

	class Ingame: public IGameState
	{
		virtual void MouseMove(int _dx, int _dy) override;
		virtual void KeyDown(int _key) override;
		virtual void KeyUp(int _key) override;
		virtual void Scroll(int _delta) override;

		virtual void Render( double _time, double _deltaTime,
			Graphic::RenderTargetList& _renderTargets,
			Graphic::ShaderList& _shaders ) override;

		virtual void Update( double _time, double _deltaTime ) override;
	};

} // namespace GameStates