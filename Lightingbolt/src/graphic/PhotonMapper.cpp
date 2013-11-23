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
	}

	PhotonMapper::~PhotonMapper()
	{
		delete m_photons;
	}

	void PhotonMapper::CreateLightMap(Graphic::VertexBuffer* _individuals, double _time, Graphic::RenderTargetList& _renderTargets,
				   Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants)
	{
		// Initialize photons with the positions of all objects
		_shaders.VSPassThrough->set();
		_shaders.GSInitPhotons->set();
		_shaders.PSPhoton->set();
		Graphic::Vertex::SetLayout();
		_individuals->set();
		m_photons->enable();
		for( int i=0; i<30; ++i )
		{
			_ShaderConstants->setTime(float(_time)+i);
			_ShaderConstants->upload();
			Graphic::Device::Context->Draw( 3, 0 );
		}

		// Simulate photons
		_shaders.VSPassPhoton->set();
		_shaders.GSSimulate->set();
		Graphic::PhotonVertex::setLayout();
		for( int i=0; i<100; ++i )
		{
			m_photons->toggle();
			m_photons->draw();
		}

		m_photons->disable();
	}
		
} // namespace Graphic