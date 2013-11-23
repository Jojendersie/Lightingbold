#include <d3d11.h>

#include "Ingame.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
#include "../graphic/device.hpp"
#include "../graphic/VertexBuffer.hpp"

namespace GameStates {

Ingame::Ingame() 
{
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 3);
}

Ingame::~Ingame()
{
	delete m_vertexBuffer;
}

void Ingame::MouseMove(int _dx, int _dy)
{
}

void Ingame::KeyDown(int _key)
{
}

void Ingame::KeyUp(int _key)
{
}

void Ingame::Scroll(int _delta)
{
}

void Ingame::Render( double _time, double _deltaTime, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders)
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	// Test
	_shaders.VSPassThrough->set();
	_shaders.GSQuad->set();
	_shaders.PSBlob->set();
	Graphic::Vertex::SetLayout();

	m_vertexBuffer->set();

	Graphic::Device::Context->Draw( 3, 0 );

	Graphic::Device::Window->Present();
}

void Ingame::Update( double _time, double _deltaTime )
{
	int number = 3; //TODO: vertices
	Graphic::Vertex *vertexes= new Graphic::Vertex[number];
	for(int i = 0;i<number;i++){
		vertexes[i].Position.x = i*0.25f;
		vertexes[i].Size = 0.25;
		vertexes[i].Rotation.y = 1.0;
	}
	m_vertexBuffer->upload(vertexes, number);
	delete[] vertexes;
}

} // namespace GameStates