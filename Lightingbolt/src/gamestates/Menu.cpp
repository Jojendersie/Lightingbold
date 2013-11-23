#include <d3d11.h>

#include "Menu.hpp"
#include "../graphic/device.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/Vertex.hpp"
#include "../graphic/device.hpp"
#include "../graphic/VertexBuffer.hpp"
#include "../graphic/TransformFeedbackBuffer.hpp"

namespace GameStates {

Menu::Menu()
{
	m_photons = new Graphic::FeedBackBuffer();
	m_vertexBuffer = new Graphic::VertexBuffer(sizeof(Graphic::Vertex), 3);
	int number = 3;
	Graphic::Vertex vertices[3];
	for(int i = 0;i<number;i++){
		vertices[i].Position.x = -i*0.25f;
		vertices[i].Size = 0.25;
		vertices[i].Rotation.y = 1.0;
	}
	m_vertexBuffer->upload(vertices, number);
}

Menu::~Menu()
{
	delete m_vertexBuffer;
	delete m_photons;
}

void Menu::MouseMove(int _dx, int _dy)
{
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
				   Graphic::ShaderList& _shaders)
{
	_renderTargets.BackBuffer->SetAsTarget();
	_renderTargets.BackBuffer->Clear( CLEAR_COLOR );

	// Initialize photons with the positions of all objects
	_shaders.VSPassThrough->set();
	_shaders.GSInitPhotons->set();
	_shaders.PSPhoton->set();
	Graphic::Vertex::SetLayout();
	m_vertexBuffer->set();
	m_photons->enable();
	Graphic::Device::Context->Draw( 3, 0 );

	HRESULT hr;
	D3D11_QUERY_DESC Desc;
	Desc.Query = D3D11_QUERY_SO_STATISTICS;
	Desc.MiscFlags = 0;
	ID3D11Query* query;
	hr = Graphic::Device::Device->CreateQuery( &Desc, &query );
	Graphic::Device::Context->Begin( query );


	// Simulate photons
	_shaders.VSPassPhoton->set();
	_shaders.GSSimulate->set();
	Graphic::PhotonVertex::setLayout();
	for( int i=0; i<10; ++i )
	{
		m_photons->toggle();
		m_photons->draw();
	}

	m_photons->disable();

	Graphic::Device::Context->End( query );
	D3D11_QUERY_DATA_SO_STATISTICS stat;
	while(S_FALSE == Graphic::Device::Context->GetData( query, &stat, sizeof(D3D11_QUERY_DATA_SO_STATISTICS), 0 ));

	query->Release();

	Graphic::Device::Window->Present();
}

void Menu::Update( double _time, double _deltaTime )
{
}

} // namespace GameStates