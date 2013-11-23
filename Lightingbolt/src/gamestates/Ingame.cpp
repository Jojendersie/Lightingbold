#include <d3d11.h>

#include "Ingame.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
#include "../graphic/device.hpp"

namespace GameStates {

Ingame::Ingame() 
{
	int number = 3; //TODO: vertices
	Graphic::Vertex *vertexes= new Graphic::Vertex[number];
	for(int i = 0;i<number;i++){
		vertexes[i].Position.x = i*0.25f;
		vertexes[i].Size = 0.25;
		vertexes[i].Rotation.y = 1.0;
	}
	m_vertexBuffer = Graphic::Device::Window->CreateStaticStdBuffer(number*sizeof(Graphic::Vertex),vertexes,D3D11_BIND_VERTEX_BUFFER);
	delete[] vertexes;
}

Ingame::~Ingame()
{
	m_vertexBuffer->Release();
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
	_shaders.VSPassThrough->Set();
	_shaders.GSQuad->Set();
	_shaders.PSBlob->Set();
	Graphic::Vertex::SetLayout();

	uint stride = sizeof(Graphic::Vertex);
	uint offset = 0;
	Graphic::Device::Context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	Graphic::Device::Context->Draw( 3, 0 );

	Graphic::Device::Window->Present();
}

void Ingame::Update( double _time, double _deltaTime )
{

}

} // namespace GameStates