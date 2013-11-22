#include "Menu.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"

namespace GameStates {

void Menu::MouseMove(int _dx, int _dy)
{
}

void Menu::KeyDown(int _key)
{
}

void Menu::KeyUp(int _key)
{
}

void Menu::Scroll(int _delta)
{
}

void Menu::Render( double _time, double _deltaTime, Graphic::RenderTargetList& _renderTargets )
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );
	Graphic::Device::Window->Present();
}

} // namespace GameStates