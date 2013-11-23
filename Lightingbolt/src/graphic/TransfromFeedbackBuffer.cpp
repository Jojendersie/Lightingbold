#include <d3d11.h>
#include "TransformFeedbackBuffer.hpp"
#include "Vertex.hpp"

namespace Graphic {

	FeedBackBuffer::FeedBackBuffer() :
		m_source(0)
	{
		// TODO other usage (no CPU updates)
		D3D11_BUFFER_DESC bufferDesc =
		{
			sizeof(PhotonVertex) * 1000,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};
		// Create two identic buffers to toggle in between
		Device::Device->CreateBuffer( &bufferDesc, nullptr, &m_buffers[0] );
		Device::Device->CreateBuffer( &bufferDesc, nullptr, &m_buffers[1] );
	}

	FeedBackBuffer::~FeedBackBuffer()
	{
		m_buffers[0]->Release();
		m_buffers[1]->Release();
	}

	void FeedBackBuffer::enable()
	{
		uint offset = 0;
		Device::Context->SOSetTargets( 1, &m_buffers[1-m_source], &offset );
	}

	void FeedBackBuffer::toggle()
	{
		m_source = 1-m_source;
		uint offset = 0;
		uint stride = sizeof(PhotonVertex);
		// If one of the buffers is bound to the other stage it must be unbound before.
		disable();
		Device::Context->IASetVertexBuffers( 0, 1, &m_buffers[m_source], &stride, &offset );
		Device::Context->SOSetTargets( 1, &m_buffers[1-m_source], &offset );
	}

	void FeedBackBuffer::disable()
	{
		Device::Context->SOSetTargets( 0, nullptr, nullptr );
	}

	void FeedBackBuffer::draw()
	{
		uint offset = 0;
		uint stride = sizeof(PhotonVertex);
		Device::Context->IASetVertexBuffers( 0, 1, &m_buffers[m_source], &stride, &offset );
		Device::Context->DrawAuto();
	}

} // namespace Graphic