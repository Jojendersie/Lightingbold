#include <d3d11.h>
#include "UniformBuffer.hpp"
#include "device.hpp"

namespace Graphic {

	UniformBuffer::UniformBuffer()
	{
		// Fill everything in the dx-creation descriptor. Not using a memset 0
		// is saver in respect to forgotten attributes.
		D3D11_BUFFER_DESC Desc;
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		Desc.ByteWidth = sizeof(m_storage);
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

	UniformBuffer::~UniformBuffer()
	{
		if(m_buffer) m_buffer->Release();
	}

	void UniformBuffer::upload()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		HRESULT hr = Device::Context->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
		Assert( SUCCEEDED(hr) );
		memcpy( MappedResource.pData, &m_storage, sizeof(m_storage) );
		Device::Context->Unmap( m_buffer, 0 );
	}

	void UniformBuffer::setMaterial( int _index, float _refraction )
	{
		Assert( 0<=_index && _index<8 );
		m_storage.Materials[_index].Refraction = _refraction;
	}

} // namespace Graphic