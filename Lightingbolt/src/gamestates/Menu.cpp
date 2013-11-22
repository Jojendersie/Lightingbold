#include "Menu.hpp"
#include "../graphic/device.hpp"

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

void Menu::Render( double _time, double _deltaTime )
{
	Graphic::Device::Window->Present();
}

} // namespace GameStates