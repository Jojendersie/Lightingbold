#include <d3d11.h>

#include "Menu.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
#include "../graphic/device.hpp"
#include "../graphic/VertexBuffer.hpp"
#include "../graphic/UniformBuffer.hpp"
#include "../graphic/PhotonMapper.hpp"
#include "../map/map.hpp"
#include "../ai/Enemy.hpp"

namespace GameStates {

	Map::Map* map;

Menu::Menu()
{
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 3);
	/** Test **/
	map = new Map::Map(1024,768);
	map->addEnemy(Math::Vec2(0),0.3);
	map->getEnemy(0)->setGoal(Math::Vec2(215,400));
	map->addEnemy(Math::Vec2(215,400),0.7);
	map->getEnemy(1)->setGoal(Math::Vec2(2,40));
	map->addEnemy(Math::Vec2(400,30),0.5);
	map->getEnemy(2)->setGoal(Math::Vec2(500,10));
	/**********/
//	m_mapTexture = new Graphic::RenderTarget( map->getWidth(), map->getHeight(), , map->getDensityMap() );

	m_photonMapper = new Graphic::PhotonMapper( 4, 100 );
}

Menu::~Menu()
{
	delete m_vertexBuffer;
	delete m_photonMapper;
	delete map;
}

void Menu::MouseMove(int _dx, int _dy)
{
	map->getEnemy(0)->setGoal(Math::Vec2(_dx,-_dy));
}

void Menu::KeyDown(int _key)
{
	if( _key == 13 ) SwitchGameState(GameStates::GS::INGAME);
}

void Menu::KeyUp(int _key)
{
}

void Menu::Scroll(int _delta)
{
}

void Menu::Render( double _time, double _deltaTime, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants)
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	//_time *= 0;
	// Fill relevant constants
//	_ShaderConstants->setTime(float(_time));
	//_ShaderConstants->upload();

	m_photonMapper->CreateLightMap( m_vertexBuffer, _time, _renderTargets, _shaders, _ShaderConstants );

	Graphic::Device::Window->Present();
}

void Menu::Update( double _time, double _deltaTime )
{
	map->Update();
	int number = map->getNumberOfObjects(); //TODO: vertices
	Graphic::Vertex *vertexes= new Graphic::Vertex[number];
	for(int i = 0;i<number;i++){
		//vertexes[i].Position.x = i*0.25f;
		//vertexes[i].Size = 0.25;
		vertexes[i].Position.x = ((map->getEnemy(i)->getPosition().x)/1024) * 2;
		vertexes[i].Position.y = ((map->getEnemy(i)->getPosition().y)/768) * 2;
		vertexes[i].Size = map->getEnemy(i)->getRadius();
		vertexes[i].Rotation.y = 1.0;
	}
	m_vertexBuffer->upload(vertexes, number);
	delete[] vertexes;
}

} // namespace GameStates