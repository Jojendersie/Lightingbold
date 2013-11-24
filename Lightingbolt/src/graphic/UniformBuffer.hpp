#pragma once

#include "../math/math.hpp"

namespace Graphic {

	/// \brief The one hard coded uniform buffer
	class UniformBuffer
	{
	public:
		struct Material {
			Math::Vec3 Color;
			float Refraction;
		};

		/// \brief Create the directx resource for this buffer.
		UniformBuffer();

		~UniformBuffer();

		/// \brief Commit changes to the GPU.
		void upload();

		void setMaterial( int _index, float _refraction, const Math::Vec3& _color );
		void setTime( float _time )					{ m_storage.Time = _time; }
		void setMapSize( const Math::Vec2& _size )	{ m_storage.MapSize = _size; }
		/// \brief Determines how long (values near 1) the light stays.
		///		Negative values activate the tone mapper.
		void setLightScale( float _scale )			{ m_storage.LightScale = _scale; }

		/// \brief The player energy is the referene for all colors
		void setPlayerEnergy( float _energy )		{ m_storage.PlayerEnergy = _energy; }

		/// \brief Set delta coord (texelsize in x or y) for blur
		void setDeltaCoord( const Math::Vec2& _delta )	{ m_storage.DeltaCoord = _delta; }
	private:
		ID3D11Buffer* m_buffer;	///< DirectX constant buffer

		struct {
			float Time;
			Math::Vec2 MapSize;
			float LightScale;
			float PlayerEnergy;
			Math::Vec2 DeltaCoord;
			float _1;
			Material Materials[8];
		} m_storage;
	};

} // namespace Graphic