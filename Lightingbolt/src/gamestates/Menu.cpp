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
#include "../generator/Random.hpp"

extern Generators::Random* g_rand;

namespace GameStates {

	Map::Map* map;

Menu::Menu()
{
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 100);
	/** Test **/
	map = new Map::Map(1024,768);
	map->addEnemy(Math::Vec2(g_rand->Uniform(0.0f,0.5f),g_rand->Uniform(0.0f,0.5f)),0.05f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.07f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.011f);
	/**********/
	m_mapTexture = new Graphic::RenderTarget( map->getWidth(), map->getHeight(),
		DXGI_FORMAT_R32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW,
		map->getDensityMap() );

	m_photonMapper = new Graphic::PhotonMapper( 10, 70 );
}

Menu::~Menu()
{
	delete m_vertexBuffer;
	delete m_photonMapper;
	delete m_mapTexture;
	delete map;
}

void Menu::MouseMove(int _dx, int _dy)
{
	map->getPlayer()->setGoal(Math::Vec2((float)_dx/(1024.0f/2.0f),(float)-_dy/(768.0f/2.0f)));
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
	// Fill relevant constants
	_ShaderConstants->setMapSize( Vec2(map->getWidth(), map->getHeight()) );
	_ShaderConstants->setPlayerEnergy( map->getPlayer()->getEnergy() );
	_ShaderConstants->setMaterial(0, 0.2f, Vec3(1.0f, 0.0f, 0.0f));
	_ShaderConstants->setMaterial(1, 0.6f, Vec3(0.0f, 1.0f, 0.0f));
	_ShaderConstants->setMaterial(2, 1.0f, Vec3(0.0f, 0.0f, 1.0f));

	m_mapTexture->SetAsTexture(0);
	m_photonMapper->CreateLightMap( m_vertexBuffer, _time, _renderTargets, _shaders, _ShaderConstants );

	// Draw light map to framebuffer
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	_ShaderConstants->setLightScale(-0.5f);	// Tone mapper
	_ShaderConstants->upload();
	Graphic::Device::Window->drawScreenQuad();

	// Draw blob stuff
	Graphic::Device::Window->setBlendMode( Graphic::DX11Window::BLEND_MODES::INV_MULT );
	_shaders.VSPassThrough->set();
	_shaders.GSQuad->set();
	_shaders.PSBlob->set();
	Graphic::Vertex::SetLayout();
	m_vertexBuffer->set();
	Graphic::Device::Context->Draw( map->getNumberOfObjects()+1, 0 );
	_renderTargets.BackBuffer->SetAsTarget();

	Graphic::Device::Window->Present();
}

void Menu::Update( double _time, double _deltaTime )
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
		vertices[i].MaterialIndex = i;
		vertices[i].ShapeIdx1 = 0;//g_rand->Uniform(0,3);
		vertices[i].ShapeIdx2 = 0;//g_rand->Uniform(0,3);
		vertices[i].Energy = map->getEnemy(i)->getEnergy();
		vertices[i].ShapeInterpolation = g_rand->Uniform();
	}

	vertices[number].Position.x = ((map->getPlayer()->getPosition().x));
	vertices[number].Position.y = ((map->getPlayer()->getPosition().y));
	vertices[number].Size = map->getPlayer()->getRadius();
	vertices[number].MaterialIndex = 1;
	vertices[number].ShapeIdx1 = 0;//g_rand->Uniform(0,3);
	vertices[number].ShapeIdx2 = 0;//g_rand->Uniform(0,3);
	vertices[number].Energy = map->getPlayer()->getEnergy();
	vertices[number].ShapeInterpolation = g_rand->Uniform();

	m_vertexBuffer->upload(vertices, number+1);
	delete[] vertices;
}

} // namespace GameStates