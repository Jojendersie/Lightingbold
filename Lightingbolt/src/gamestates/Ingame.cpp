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
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 10);
	/** Test **/
	map = new Map::Map(1024,768);
	map->addEnemy(Math::Vec2(0),0.3);
	map->getEnemy(0)->setGoal(Math::Vec2(0));
	map->addEnemy(Math::Vec2(0),0.7);
	map->getEnemy(1)->setGoal(Math::Vec2(0));
	map->addEnemy(Math::Vec2(0),0.5);
	map->getEnemy(2)->setGoal(Math::Vec2(0));
	/**********/
}

Ingame::~Ingame()
{
	delete m_vertexBuffer;
	delete map;
}

void Ingame::MouseMove(int _dx, int _dy)
{
	map->getPlayer()->setGoal(Math::Vec2((float)_dx/(1024.0f/2.0f),(float)-_dy/(768.0f/2.0f)));
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
				   Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants)
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	// Test
	_shaders.VSPassThrough->set();
	_shaders.GSQuad->set();
	_shaders.PSBlob->set();
	Graphic::Vertex::SetLayout();

	m_vertexBuffer->set();

	Graphic::Device::Context->Draw( map->getNumberOfObjects()+1, 0 );

	Graphic::Device::Window->Present();
}

void Ingame::Update( double _time, double _deltaTime )
{
	map->Update();
	int number = map->getNumberOfObjects();
	Graphic::Vertex *vertices= new Graphic::Vertex[number+1];
	for(int i = 0;i<number;i++){
		//vertices[i].Position.x = i*0.25f;
		//vertices[i].Size = 0.25;
		vertices[i].Position.x = ((map->getEnemy(i)->getPosition().x));
		vertices[i].Position.y = ((map->getEnemy(i)->getPosition().y));
		vertices[i].Size = map->getEnemy(i)->getRadius();
		vertices[i].Rotation.y = 1.0;
	}

	vertices[number].Position.x = ((map->getPlayer()->getPosition().x));
	vertices[number].Position.y = ((map->getPlayer()->getPosition().y));
	vertices[number].Size = map->getPlayer()->getRadius();
	vertices[number].Rotation.y = 1.0;

	m_vertexBuffer->upload(vertices, number+1);
	delete[] vertices;
}

} // namespace GameStates