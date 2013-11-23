#pragma once
#include "Vertex.hpp"

namespace Graphic {

	/// \brief The one hard coded vertex buffer
	class VertexBuffer
	{
	public:

		/// \brief Create the directx resource for this buffer.
		VertexBuffer();

		~VertexBuffer();

		//ID3D11Buffer* getBuffer();

		void set();

		void setVertices(Vertex* _vertices);

		/// \brief Commit changes to the GPU.
		void upload();

	private:
		ID3D11Buffer* m_buffer;	///< DirectX constant buffer
		Vertex* m_vertices;
	};

} // namespace Graphic