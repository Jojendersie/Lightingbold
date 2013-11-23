#pragma once

namespace Graphic {

	/// \brief The one hard coded uniform buffer
	class UniformBuffer
	{
	public:
		struct Material {
			float Refraction;
		};

		/// \brief Create the directx resource for this buffer.
		UniformBuffer();

		~UniformBuffer();

		/// \brief Commit changes to the GPU.
		void upload();

		void setMaterial( int _index, float _refraction );
		void setTime( float _time )		{ m_storage.Time = _time; }
	private:
		ID3D11Buffer* m_buffer;	///< DirectX constant buffer

		struct {
			float Time;
			float _1;
			float _2;
			float _3;
			Material Materials[8];
		} m_storage;
	};

} // namespace Graphic