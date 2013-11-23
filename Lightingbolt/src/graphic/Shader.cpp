#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "Shader.hpp"

#ifdef DYNAMIC_SHADER_RELOAD
#	include <iostream>
#	include <string>
#	include <sys/stat.h>
#endif

namespace Graphic {
	Shader::Shader( const wchar_t* _fileName, Type _type,
			D3D11_SO_DECLARATION_ENTRY* _outLayout, int _layoutSize ) :
		m_fileName(_fileName),
		m_type(_type),
		m_shader(nullptr),
		m_vertexShader(nullptr),
		m_outLayoutDesc(_outLayout),
		m_outLayoutNum(_layoutSize)
	{
		load();
	}

	Shader::~Shader()
	{
		if(m_shader) m_shader->Release();
		m_shader = nullptr;
		// Release is a basic interface method -> type unrelevant
		if(m_vertexShader) m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	void Shader::load()
	{
		HRESULT hr;

#ifdef DYNAMIC_SHADER_RELOAD
		// Unload if there was one before
		this->~Shader();

		struct _stat64i32 fileStatus;
		_wstat( m_fileName, &fileStatus );
		m_lastModified = fileStatus.st_mtime;
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
		hr = D3DCompileFromFile( m_fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
								 "main", profile,
								 flags, 0, &m_shader, &errorBlob );

		if(FAILED(hr))
		{
#ifdef DYNAMIC_SHADER_RELOAD
			// If the shader failed to compile it should have writen something to the error message.
			if(errorBlob)
			{
				std::cerr << (char*)(errorBlob->GetBufferPointer());
				errorBlob->Release();
			} else
				std::cerr << "Shader file '" << m_fileName << "' not found.\n";
#endif
		} else {
#ifdef DYNAMIC_SHADER_RELOAD
			std::cout << "Successfully loaded shader.\n";
#endif
			switch( m_type )
			{
			case Type::VERTEX: hr = Device::Device->CreateVertexShader( m_shader->GetBufferPointer(), m_shader->GetBufferSize(), nullptr, &m_vertexShader ); break;
			case Type::GEOMETRY:
				if( m_outLayoutDesc )
				{
					Device::Device->CreateGeometryShaderWithStreamOutput( m_shader->GetBufferPointer(), m_shader->GetBufferSize(),
						m_outLayoutDesc, m_outLayoutNum, nullptr, 0, 0, nullptr, &m_geometryShader );
				} else
					hr = Device::Device->CreateGeometryShader( m_shader->GetBufferPointer(), m_shader->GetBufferSize(), nullptr, &m_geometryShader );
				break;
			case Type::PIXEL: hr = Device::Device->CreatePixelShader( m_shader->GetBufferPointer(), m_shader->GetBufferSize(), nullptr, &m_pixelShader ); break;
			} 

			Assert( SUCCEEDED(hr) );
		}
	}


	void Shader::set()
	{
		switch( m_type )
		{
		case Type::VERTEX: Device::Context->VSSetShader( m_vertexShader, nullptr, 0 ); break;
		case Type::GEOMETRY: Device::Context->GSSetShader( m_geometryShader, nullptr, 0 ); break;
		case Type::PIXEL: Device::Context->PSSetShader( m_pixelShader, nullptr, 0 ); break;
		}
	}

#ifdef DYNAMIC_SHADER_RELOAD
	void Shader::dynamicReload()
	{
		// Find out if the file was changed
		struct _stat64i32 fileStatus;
		_wstat( m_fileName, &fileStatus );

		if( fileStatus.st_mtime != m_lastModified )
		{
			m_lastModified = fileStatus.st_mtime;

			// Unload old and reload (both done by load)
			load();
		}
	}
#endif

} // namespace Graphic