#include <d3d11.h>
#include "Vertex.hpp"
#include "Shader.hpp"

namespace Graphic {

	ID3D11InputLayout* g_VertexLayout;

	void Vertex::InitLayout(Shader* _shader)
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		HRESULT hr = Device::Device->CreateInputLayout(
			LayoutDesc, 4, _shader->m_shader->GetBufferPointer(), _shader->m_shader->GetBufferSize(), &g_VertexLayout);
		Assert( SUCCEEDED(hr) );
	}

	void Vertex::ReleaseLayout()
	{
		g_VertexLayout->Release();
	}

	void Vertex::SetLayout()
	{
		Device::Context->IASetInputLayout( g_VertexLayout );
	}

} // namespace Graphic