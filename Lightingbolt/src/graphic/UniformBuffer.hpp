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
	private:
		ID3D11Buffer* m_buffer;	///< DirectX constant buffer

		struct {
			Material Materials[8];
		} m_storage;
	};

} // namespace Graphic