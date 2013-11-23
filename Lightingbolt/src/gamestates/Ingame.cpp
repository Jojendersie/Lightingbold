#include <d3d11.h>

#include "Ingame.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
#include "../graphic/device.hpp"
#include "../graphic/VertexBuffer.hpp"
#include "../map/map.hpp"
#include "../ai/Enemy.hpp"

namespace GameStates {

Ingame::Ingame() 
{
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 3);
	/** Test **/
	map = new Map::Map(512,512);
	map->addEnemy(Math::Vec2(0),0.3);
	map->addEnemy(Math::Vec2(215,400),0.7);
	map->addEnemy(Math::Vec2(400,30),0.5);
	/**********/
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
	_shaders.VSPassThrough->Set();
	_shaders.GSQuad->Set();
	_shaders.PSBlob->Set();
	Graphic::Vertex::SetLayout();

	m_vertexBuffer->set();

	Graphic::Device::Context->Draw( 3, 0 );

	Graphic::Device::Window->Present();
}

void Ingame::Update( double _time, double _deltaTime )
{
	map->Update();
	int number = map->getNumberOfObjects(); //TODO: vertices
	Graphic::Vertex *vertexes= new Graphic::Vertex[number];
	for(int i = 0;i<number;i++){
		//vertexes[i].Position.x = i*0.25f;
		//vertexes[i].Size = 0.25;
		vertexes[i].Position.x = (map->getEnemy(i)->getPosition().x -256)/512;
		vertexes[i].Position.y = (map->getEnemy(i)->getPosition().y -256)/512;
		vertexes[i].Size = map->getEnemy(i)->getRadius();
		vertexes[i].Rotation.y = 1.0;
	}
	m_vertexBuffer->upload(vertexes, number);
	delete[] vertexes;
}

} // namespace GameStates