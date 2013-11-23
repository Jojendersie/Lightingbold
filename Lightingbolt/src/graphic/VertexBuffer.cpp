#include <d3d11.h>
#include "VertexBuffer.hpp"
#include "device.hpp"

namespace Graphic {

	VertexBuffer::VertexBuffer(int _vertexSize, int _maxNum) :
		m_vertexSize(_vertexSize),
		m_maxNum(_maxNum)
	{	
		// Fill everything in the dx-creation descriptor. Not using a memset 0
		// is saver in respect to forgotten attributes.
		D3D11_BUFFER_DESC Desc;
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		Desc.ByteWidth = _vertexSize * _maxNum;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Desc.MiscFlags = 0;
		Desc.StructureByteStride = 0;
		Desc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT hr = Graphic::Device::Device->CreateBuffer( &Desc, nullptr, &m_buffer );
		Assert( SUCCEEDED(hr) );
	}



	void VertexBuffer::set(){
		uint offset = 0;
		Graphic::Device::Context->IASetVertexBuffers(0, 1, &m_buffer,&m_vertexSize, &offset);
	}

	VertexBuffer::~VertexBuffer()
	{
		if(m_buffer) m_buffer->Release();
	}

	/*ID3D11Buffer* VertexBuffer::getBuffer(){
		return m_buffer;
	}*/

	void VertexBuffer::upload(void* _vertices, int _numVertices)
	{
		Assert(_numVertices <= m_maxNum);
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		HRESULT hr = Device::Context->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
		Assert( SUCCEEDED(hr) );
		memcpy( MappedResource.pData, _vertices, _numVertices * m_vertexSize );
		Device::Context->Unmap( m_buffer, 0 );
	}

} // namespace Graphic