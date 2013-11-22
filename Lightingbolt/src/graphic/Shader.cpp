#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "Shader.hpp"

#ifdef DYNAMIC_SHADER_RELOAD
#include <iostream>
#include <string>
#endif

namespace Graphic {
	Shader::Shader( const wchar_t* _fileName, Type _type ) :
		m_fileName(_fileName),
		m_type(_type),
		m_shader(nullptr)
	{
		Load();
	}

	Shader::~Shader()
	{
		if(m_shader) m_shader->Release();
	}

	void Shader::Load()
	{
#ifdef DYNAMIC_SHADER_RELOAD
		// Unload if there was one before
		if(m_shader) m_shader->Release();
#endif


		uint flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif
		// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.
		LPCSTR profile = "cs_5_0";
		switch( m_type )
		{
		case Type::VERTEX: profile = ( Device::Device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "vs_5_0" : "vs_4_0"; break;
		case Type::GEOMETRY: profile = ( Device::Device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "gs_5_0" : "gs_4_0"; break;
		case Type::PIXEL: profile = ( Device::Device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "ps_5_0" : "ps_4_0"; break;
		}

		ID3DBlob* errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile( m_fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
										 "main", profile,
										 flags, 0, &m_shader, &errorBlob );
#ifdef DYNAMIC_SHADER_RELOAD
		if(FAILED(hr))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if(errorBlob)
			{
				std::cerr << (char*)(errorBlob->GetBufferPointer());
				errorBlob->Release();
			} else
				std::cerr << "Shader file '" << m_fileName << "' not found.\n";
		}
#endif
	}

} // namespace Graphic