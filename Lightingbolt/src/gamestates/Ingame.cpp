#include <d3d11.h>

#include "Ingame.hpp"
#include "../graphic/device.hpp"
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

Ingame::Ingame() 
{
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 50);
	/** Test **/
	map = new Map::Map(Graphic::Device::Window->Width(),Graphic::Device::Window->Height());
	/*map->addEnemy(Math::Vec2(g_rand->Uniform(0.0f,0.5f),g_rand->Uniform(0.0f,0.5f)),0.05f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.07f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.011f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.511f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.731f);
	map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),0.1f);*/
	m_minEnemies = 4;
	m_maxEnemies = 20;
	int numberOfEnemies = g_rand->Uniform(m_minEnemies,m_maxEnemies);
	for(int i=0;i<numberOfEnemies;++i)
	{
		map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),g_rand->Uniform(0.08f,map->getPlayer()->getEnergy()-0.05f));
		map->getEnemy(i)->setShape((int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),(int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),g_rand->Uniform(0.0f,1.0f));
		map->getEnemy(i)->setMaterialIndex((int)floor(g_rand->Uniform(0.0f,3.0f)+0.5));
	}

	/**********/

	m_mapTexture = new Graphic::RenderTarget( map->getWidth(), map->getHeight(),
		DXGI_FORMAT_R32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW,
		map->getDensityMap() );

	m_photonMapper = new Graphic::PhotonMapper( 4, 100 );
}

Ingame::~Ingame()
{
	delete m_vertexBuffer;
	delete m_photonMapper;
	delete m_mapTexture;
	delete map;
}

void Ingame::MouseMove(int _dx, int _dy)
{
	map->getPlayer()->setGoal(Math::Vec2((float)_dx/(Graphic::Device::Window->Width()/2.0f),
		(float)-_dy/(Graphic::Device::Window->Height()/2.0f)));
}

void Ingame::KeyDown(int _key)
{
	//if(!map->getPlayer()->isAlive())
	if( _key == 13 )
	{
		SwitchGameState(GameStates::GS::INGAME);
	}
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
	// Fill relevant constants
	_ShaderConstants->setMapSize( Vec2(map->getWidth(), map->getHeight()) );
	_ShaderConstants->setPlayerEnergy( map->getPlayer()->getEnergy() );
	_ShaderConstants->setMaterial(0, 0.0f, Vec3(1.0f, 0.0f, 0.0f));
	_ShaderConstants->setMaterial(1, 1.0f, Vec3(0.0f, 1.0f, 0.0f));
	_ShaderConstants->setMaterial(2, 1.0f, Vec3(0.0f, 0.25f, 1.0f));
	_ShaderConstants->setMaterial(4, 1.0f, Vec3(0.0f, 0.75f, 1.0f));

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

	Graphic::Device::Window->Present();
}

void Ingame::Update( double _time, double _deltaTime )
{
	if(map->getNumberOfObjects() < m_maxEnemies)
	{
		/*if(map->getNumberOfObjects() < m_minEnemies*3)
		{
			if(g_rand->Uniform(0.0f,1.0f)<0.05f)
			{
				map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),g_rand->Uniform(0.08f,map->getPlayer()->getEnergy()-0.05f));
			}
		}*/
		if(map->getNumberOfObjects() < m_minEnemies*2)
		{
			if(g_rand->Uniform(0.0f,1.0f)<0.2f)
			{
				map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),g_rand->Uniform(0.08f,map->getPlayer()->getEnergy()+0.15f));
				map->getEnemy(map->getNumberOfObjects()-1)->setShape((int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),(int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),g_rand->Uniform(0.0f,1.0f));
				map->getEnemy(map->getNumberOfObjects()-1)->setMaterialIndex((g_rand->Uniform(0,3)));
			}
		}
		else if(map->getNumberOfObjects() < m_minEnemies)
		{
			if(g_rand->Uniform(0.0f,1.0f)<0.4f)
			{
				map->addEnemy(Math::Vec2(g_rand->Uniform(),g_rand->Uniform()),g_rand->Uniform(0.08f,map->getPlayer()->getEnergy()+0.15f));
				map->getEnemy(map->getNumberOfObjects()-1)->setShape((int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),(int)floor(g_rand->Uniform(0.0f,3.0f)+0.5),g_rand->Uniform(0.0f,1.0f));
				map->getEnemy(map->getNumberOfObjects()-1)->setMaterialIndex((g_rand->Uniform(0,3)));
			}
		}
	}

	map->Update(_deltaTime);
	int number = map->getNumberOfObjects();
	Graphic::Vertex *vertices= new Graphic::Vertex[number+1];
	for(int i = 0;i<number;i++){
		//vertices[i].Position.x = i*0.25f;
		//vertices[i].Size = 0.25;
		vertices[i].Position.x = ((map->getEnemy(i)->getPosition().x));
		vertices[i].Position.y = ((map->getEnemy(i)->getPosition().y));
		vertices[i].Size = map->getEnemy(i)->getRadius();
		vertices[i].Size.y *= Graphic::Device::Window->Width() / float(Graphic::Device::Window->Height());
		vertices[i].MaterialIndex =map->getEnemy(i)->getMaterialIndex();
		vertices[i].ShapeIdx1 = map->getEnemy(i)->getShape1();//g_rand->Uniform(0,3);
		vertices[i].ShapeIdx2 = map->getEnemy(i)->getShape2();//g_rand->Uniform(0,3);
		vertices[i].Energy = map->getEnemy(i)->getEnergy();
		vertices[i].ShapeInterpolation = map->getEnemy(i)->getshapeInterpolation();//g_rand->Uniform();
	}

	vertices[number].Position.x = ((map->getPlayer()->getPosition().x));
	vertices[number].Position.y = ((map->getPlayer()->getPosition().y));
	vertices[number].Size = map->getPlayer()->getRadius();
	vertices[number].Size.y *= Graphic::Device::Window->Width() / float(Graphic::Device::Window->Height());
	vertices[number].MaterialIndex = map->getPlayer()->getMaterialIndex();
	vertices[number].ShapeIdx1 = map->getPlayer()->getShape1();//g_rand->Uniform(0,3);
	vertices[number].ShapeIdx2 = map->getPlayer()->getShape2();//g_rand->Uniform(0,3);
	vertices[number].Energy = map->getPlayer()->getEnergy();
	vertices[number].ShapeInterpolation = map->getPlayer()->getshapeInterpolation();//g_rand->Uniform();

	m_vertexBuffer->upload(vertices, number+1);
	delete[] vertices;
}

} // namespace GameStates