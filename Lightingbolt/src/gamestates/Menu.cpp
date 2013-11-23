#include <d3d11.h>

#include "Menu.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
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

void Menu::Render( double _time, double _deltaTime, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders)
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	// Test
	_shaders.VSPassThrough->Set();
	_shaders.GSQuad->Set();
	_shaders.PSBlob->Set();
	Graphic::Vertex::SetLayout();
	Graphic::Device::Context->Draw( 1, 0 );

	Graphic::Device::Window->Present();
}

} // namespace GameStates