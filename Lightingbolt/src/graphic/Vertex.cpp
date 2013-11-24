#include <d3d11.h>
#include "Vertex.hpp"
#include "Shader.hpp"

namespace Graphic {

	ID3D11InputLayout* g_VertexLayout;
	ID3D11InputLayout* g_PhotonVertexLayout;

	void Vertex::InitLayout(Shader* _shader)
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R8G8B8A8_UINT, 0, sizeof(float)*6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		HRESULT hr = Device::Device->CreateInputLayout(
			LayoutDesc, 4, _shader->m_shaderCode, _shader->m_codeSize, &g_VertexLayout);
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


	// *** PHOTON VERTEX *************************************************** //
	void PhotonVertex::initLayout(Shader* _shader)
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		HRESULT hr = Device::Device->CreateInputLayout(
			LayoutDesc, 3, _shader->m_shaderCode,
			_shader->m_codeSize, &g_PhotonVertexLayout);
		Assert( SUCCEEDED(hr) );
	}

	void PhotonVertex::releaseLayout()
	{
		g_PhotonVertexLayout->Release();
	}

	void PhotonVertex::setLayout()
	{
		Device::Context->IASetInputLayout( g_PhotonVertexLayout );
	}

	D3D11_SO_DECLARATION_ENTRY* PhotonVertex::getOutputLayoutDesc()
	{
		static D3D11_SO_DECLARATION_ENTRY pDecl[] =
		{
			// stream, semantic name, semantic index, start component, component count, output slot
			{ 0, "SV_POSITION", 0, 0, 2, 0 },   // output two components of position
			{ 0, "TEXCOORD", 0, 0, 2, 0 },     // output the direction
			{ 0, "TEXCOORD", 1, 0, 3, 0 },     // output the energy
		};
		return pDecl;
	}

	int PhotonVertex::getOutputLayoutNum()
	{
		return 3;
	}

} // namespace Graphic