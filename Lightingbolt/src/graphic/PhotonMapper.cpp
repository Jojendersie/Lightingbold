#include <d3d11.h>

#include "PhotonMapper.hpp"
#include "../graphic/TransformFeedbackBuffer.hpp"
#include "../graphic/VertexBuffer.hpp"
#include "../graphic/UniformBuffer.hpp"
#include "../graphic/RenderTarget.hpp"
#include "../graphic/Shader.hpp"
#include "../graphic/device.hpp"

namespace Graphic {

	PhotonMapper::PhotonMapper( int _numPhotons, int _simSteps ) :
		m_numPhotons(_numPhotons),
		m_simSteps(_simSteps)
	{
		m_photons = new Graphic::FeedBackBuffer();
		m_photonMap[0] = new Graphic::RenderTarget( 512, 512, DXGI_FORMAT_R32G32B32A32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_photonMap[1] = new Graphic::RenderTarget( 512, 512, DXGI_FORMAT_R32G32B32A32_FLOAT, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH | Graphic::RenderTarget::CREATION_FLAGS::TARGET_TEXTURE_VIEW, nullptr );
		m_targetMap = 0;
	}

	PhotonMapper::~PhotonMapper()
	{
		delete m_photonMap[0];
		delete m_photonMap[1];
		delete m_photons;
	}

	void PhotonMapper::CreateLightMap(Graphic::VertexBuffer* _individuals, double _time, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants)
	{
		m_photonMap[m_targetMap]->SetAsTarget();
		m_photonMap[m_targetMap]->Clear(CLEAR_COLOR);

		Graphic::Device::Window->setBlendMode( Graphic::DX11Window::BLEND_MODES::ADDITIVE );

		// Initialize photons with the positions of all objects
		_shaders.VSPassThrough->set();
		_shaders.GSInitPhotons->set();
		_shaders.PSPhoton->set();
		Graphic::Vertex::SetLayout();
		_individuals->set();
		m_photons->enable();
		for( int i=0; i<m_numPhotons; ++i )
		{
			_ShaderConstants->setTime(float(_time)+i);
			_ShaderConstants->upload();
			Graphic::Device::Context->Draw( _individuals->getNum(), 0 );
		}

		// Simulate photons
		_shaders.VSPassPhoton->set();
		_shaders.GSSimulate->set();
		Graphic::PhotonVertex::setLayout();
		for( int i=0; i<m_simSteps; ++i )
		{
			m_photons->toggle();
			m_photons->draw();
		}
		m_photons->disable();

		// Combine with last map (scaled)
		m_photonMap[1-m_targetMap]->SetAsTexture(1);
		_shaders.VSPassThrough->set();
		_shaders.GSQuad->set();
		_shaders.PSTexture->set();
		_ShaderConstants->setLightScale(0.98f);
		_ShaderConstants->upload();
		Graphic::Device::Window->drawScreenQuad();

		Device::Context->OMSetRenderTargets( 0, nullptr, nullptr );
		m_photonMap[m_targetMap]->SetAsTexture(1);
		m_targetMap = 1-m_targetMap;
	}
		
} // namespace Graphic