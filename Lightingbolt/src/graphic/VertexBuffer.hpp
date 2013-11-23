#pragma once
#include "Vertex.hpp"

namespace Graphic {

	/// \brief The one hard coded vertex buffer
	class VertexBuffer
	{
	public:

		/// \brief Create the directx resource for this buffer.
		VertexBuffer( int _vertexSize, int _maxNum );

		~VertexBuffer();

		//ID3D11Buffer* getBuffer();

		void set();

		/// \brief Commit changes to the GPU.
		void upload(void* _vertices, int _numVertices);

	private:
		ID3D11Buffer* m_buffer;	///< DirectX constant buffer
		unsigned m_vertexSize;
		int m_maxNum;
	};

} // namespace Graphic