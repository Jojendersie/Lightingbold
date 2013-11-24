#include <d3d11.h>

#include "PhotonMapper.hpp"
#include "../graphic/TransformFeedbackBuffer.hpp"
#include "../graphic/VertexBuffer.hpp"
#include "../graphic/UniformBuffer.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/device.hpp"

const int MAP_RESOLUTION = 2048;
const int PHOTON_MAP_RESOLUTION = 512;

namespace Graphic {

	PhotonMapper::PhotonMapper( int _numPhotons, int _simSteps ) :
		m_numPhotons(_numPhotons),
		m_simSteps(_simSteps)
	{
		m_photons = new Graphic::FeedBackBuffer();
		m_photonMap[0] = new Graphic::RenderTarget( PHOTON_MAP_RESOLUTION, PHOTON_MAP_RESOLUTION, DXGI_FORMAT_R32G32B32A32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_photonMap[1] = new Graphic::RenderTarget( PHOTON_MAP_RESOLUTION, PHOTON_MAP_RESOLUTION, DXGI_FORMAT_R32G32B32A32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_blurMap = new Graphic::RenderTarget( PHOTON_MAP_RESOLUTION, PHOTON_MAP_RESOLUTION, DXGI_FORMAT_R32G32B32A32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_refractionTexture = new Graphic::RenderTarget( MAP_RESOLUTION, MAP_RESOLUTION, DXGI_FORMAT_R8G8B8A8_UNORM, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_targetMap = 0;
	}

	PhotonMapper::~PhotonMapper()
	{
		delete m_refractionTexture;
		delete m_photonMap[0];
		delete m_photonMap[1];
		delete m_photons;
		delete m_blurMap;
	}

	const float CLEAR_REFRACTION[4] = {0.5, 0.5, 0.5, 1.0};

	void PhotonMapper::CreateLightMap(Graphic::VertexBuffer* _individuals, double _time, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants)
	{
		// Create the refraction map
		m_refractionTexture->SetAsTarget();
		m_refractionTexture->Clear(CLEAR_REFRACTION);
		Graphic::Device::Window->setBlendMode( Graphic::DX11Window::BLEND_MODES::ALPHA );
		_shaders.VSPassThrough->set();
		_shaders.GSQuad->set();
		_shaders.PSRefractionMap->set();
		Graphic::Vertex::SetLayout();
		_individuals->set();
		Graphic::Device::Context->Draw( _individuals->getNum(), 0 );

		m_photonMap[m_targetMap]->SetAsTarget();
		m_photonMap[m_targetMap]->Clear(CLEAR_COLOR);

		Graphic::Device::Window->setBlendMode( Graphic::DX11Window::BLEND_MODES::ADDITIVE );

		for( int i=0; i<m_numPhotons; ++i ) {
			// Initialize photons with the positions of all objects
			_shaders.VSPassThrough->set();
			_shaders.GSInitPhotons->set();
			_shaders.PSPhoton->set();
			_individuals->set();
			Graphic::Vertex::SetLayout();
			m_photons->enable();
			_ShaderConstants->setTime(float(_time)+i*0.1356252f);
			_ShaderConstants->upload();
			Graphic::Device::Context->Draw( _individuals->getNum(), 0 );


			// Simulate photons
			_shaders.VSPassPhoton->set();
			_shaders.GSSimulate->set();
			Graphic::PhotonVertex::setLayout();
			m_refractionTexture->SetAsTexture(2);
			for( int i=0; i<m_simSteps; ++i )
			{
				m_photons->toggle();
				m_photons->draw();
			}
			m_photons->disable();
		}

		// Blur current step
		_shaders.VSPassThrough->set();
		_shaders.GSQuad->set();
		_shaders.PSBlur->set();
		m_blurMap->SetAsTarget();
		m_photonMap[m_targetMap]->SetAsTexture(3);
		m_blurMap->Clear(CLEAR_COLOR);
		_ShaderConstants->setDeltaCoord(Math::Vec2(1.0f/PHOTON_MAP_RESOLUTION,0.0f));
		_ShaderConstants->upload();
		Graphic::Device::Window->drawScreenQuad();
		// Pass 2
		m_photonMap[m_targetMap]->SetAsTarget();
		m_blurMap->SetAsTexture(3);
		m_photonMap[m_targetMap]->Clear(CLEAR_COLOR);
		_ShaderConstants->setDeltaCoord(Math::Vec2(0.0f, 1.0f/PHOTON_MAP_RESOLUTION));
		_ShaderConstants->upload();
		Graphic::Device::Window->drawScreenQuad();

		// Combine with last map (scaled)
		m_photonMap[1-m_targetMap]->SetAsTexture(1);
		_shaders.PSTexture->set();
		_ShaderConstants->setLightScale(0.96f);
		_ShaderConstants->upload();
		Graphic::Device::Window->drawScreenQuad();

		//m_refractionTexture->SetAsTexture(1);
		//Graphic::Device::Window->drawScreenQuad();

		Device::Context->OMSetRenderTargets( 0, nullptr, nullptr );
		m_photonMap[m_targetMap]->SetAsTexture(1);
		m_targetMap = 1-m_targetMap;
	}
		
} // namespace Graphic