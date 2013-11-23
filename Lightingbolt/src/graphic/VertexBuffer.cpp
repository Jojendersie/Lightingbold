#include <d3d11.h>
#include "VertexBuffer.hpp"
#include "device.hpp"

namespace Graphic {

	VertexBuffer::VertexBuffer()
	{	
		// Fill everything in the dx-creation descriptor. Not using a memset 0
		// is saver in respect to forgotten attributes.
		D3D11_BUFFER_DESC Desc;
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		Desc.ByteWidth = sizeof(m_vertices);
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Desc.MiscFlags = 0;
		Desc.StructureByteStride = 0;
		Desc.Usage = D3D11_USAGE_DYNAMIC;
		HRESULT hr = Graphic::Device::Device->CreateBuffer( &Desc, nullptr, &m_buffer );
		Assert( SUCCEEDED(hr) );

		// Bind similar to openGL
		Device::Context->VSSetConstantBuffers( 0, 1, &m_buffer );
		Device::Context->GSSetConstantBuffers( 0, 1, &m_buffer );
		Device::Context->PSSetConstantBuffers( 0, 1, &m_buffer );
	}



	void VertexBuffer::set(){
		uint stride = sizeof(Graphic::Vertex);
		uint offset = 0;
		Graphic::Device::Context->IASetVertexBuffers(0, 1, &m_buffer,&stride, &offset);
	}

	VertexBuffer::~VertexBuffer()
	{
		if(m_buffer) m_buffer->Release();
	}

	/*ID3D11Buffer* VertexBuffer::getBuffer(){
		return m_buffer;
	}*/

	void VertexBuffer::upload()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		HRESULT hr = Device::Context->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
		Assert( SUCCEEDED(hr) );
		memcpy( MappedResource.pData, &m_vertices, sizeof(m_vertices) );
		Device::Context->Unmap( m_buffer, 0 );
	}

	void VertexBuffer::setVertices(Vertex* _vertices)
	{
		m_vertices=_vertices;
	}

} // namespace Graphic