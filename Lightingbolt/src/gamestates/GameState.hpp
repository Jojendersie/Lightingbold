#pragma once

namespace GameStates {
	class IGameState
	{
	public:
		virtual void MouseMove(int _dx, int _dy)	{}
		virtual void KeyDown(int _key)				{}
		virtual void KeyUp(int _key)				{}
		virtual void Scroll(int _delta)				{}

		virtual void Render( double _time, double _deltaTime ) = 0;
	};
} // namespace GameStates