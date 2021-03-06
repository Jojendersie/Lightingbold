#pragma once

#include "../predecl.hpp"

namespace Graphic {

	///\brief Multipass renderer to do a 2D photon mapping
	class PhotonMapper
	{
	public:
		/// \brief Initialize the photon mapper.
		/// \param [in] _numPhotons A multiplicator for the number of photons.
		///		_numPhotons * 100 photons per individual per pass are created.
		/// \param [in] _simSteps How long can a photon path be? Common is ~100.
		PhotonMapper( int _numPhotons, int _simSteps );

		~PhotonMapper();

		/// \brief Simulate the photons.
		/// \details At the end the light map is set as texture 1.
		void CreateLightMap(Graphic::VertexBuffer* _individuals, double _time,
			Graphic::RenderTargetList& _renderTargets,
			Graphic::ShaderList& _shaders, Graphic::UniformBuffer* _ShaderConstants);
	private:
		int m_numPhotons;
		int m_simSteps;

		Graphic::FeedBackBuffer* m_photons;
		Graphic::RenderTarget* m_refractionTexture;
		Graphic::RenderTarget* m_photonMap[2];
		Graphic::RenderTarget* m_blurMap;
		int m_targetMap;
	};

} // namespace Graphic