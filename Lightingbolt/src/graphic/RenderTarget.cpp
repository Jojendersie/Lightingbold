#include <d3d11.h>
#include "RenderTarget.hpp"

namespace Graphic {
	// ********************************************************************* //
	void RenderTarget::CreateViewPort( uint _width, uint _height )
	{
		m_viewPort = new D3D11_VIEWPORT;
		m_viewPort->Width = (float)_width;
		m_viewPort->Height = (float)_height;
		m_viewPort->MinDepth = 0.0f;
	    m_viewPort->MaxDepth = 1.0f;
	    m_viewPort->TopLeftX = 0;
	    m_viewPort->TopLeftY = 0;
	}

	// ********************************************************************* //
	// Fill this rendertarget with an already existing resources.
	RenderTarget::RenderTarget( ID3D11RenderTargetView* _targetView,
					ID3D11DepthStencilView* _depthStencil,
					uint _flags ) :
		m_textureView(nullptr),
		m_DSTextureView(nullptr),
		m_targetView(_targetView),
		m_depthBufferRef(_depthStencil),
		m_lastTextureSlot(0xffffffff),
		m_DSLastTextureSlot(0xffffffff)
	{
		Assert((_targetView!=nullptr) ^ ((_flags & CREATION_FLAGS::NO_TARGET) == CREATION_FLAGS::NO_TARGET));
		Assert((_depthStencil!=nullptr) ^ ((_flags & CREATION_FLAGS::NO_DEPTH) == CREATION_FLAGS::NO_DEPTH));

		// Not implemented, yet...
		Assert( (_flags & CREATION_FLAGS::DEPTH_TEXTURE_VIEW) != CREATION_FLAGS::DEPTH_TEXTURE_VIEW );
		Assert( (_flags & CREATION_FLAGS::TARGET_TEXTURE_VIEW) != CREATION_FLAGS::TARGET_TEXTURE_VIEW );

		ID3D11Texture2D* pTex;
		_targetView->GetResource( (ID3D11Resource**)&pTex );
		D3D11_TEXTURE2D_DESC Desc;
		pTex->GetDesc( &Desc );
		pTex->Release();
		
		CreateViewPort( Desc.Width, Desc.Height );
	}

	// ********************************************************************* //
	/// \brief Create a render target with the given size and format.
	RenderTarget::RenderTarget( uint _width, uint _height, DXGI_FORMAT _format, uint _flags, void* _initalData ) :
		m_textureView(nullptr),
		m_DSTextureView(nullptr),
		m_targetView(nullptr),
		m_depthBufferRef(nullptr),
		m_lastTextureSlot(0xffffffff),
		m_DSLastTextureSlot(0xffffffff)
	{
		// Contradictory parameters!
		Assert( (CREATION_FLAGS::TARGET_TEXTURE_VIEW != (_flags & CREATION_FLAGS::TARGET_TEXTURE_VIEW)) || (CREATION_FLAGS::NO_TARGET != (_flags & CREATION_FLAGS::NO_TARGET)) );
		Assert( (CREATION_FLAGS::DEPTH_TEXTURE_VIEW != (_flags & CREATION_FLAGS::DEPTH_TEXTURE_VIEW)) || (CREATION_FLAGS::NO_DEPTH != (_flags & CREATION_FLAGS::NO_DEPTH)) );

		CreateViewPort( _width, _height );

		HRESULT hr = S_OK;
		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.ArraySize = 1;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.Height = _height;
		TexDesc.Width = _width;
		TexDesc.MipLevels = 1;
		TexDesc.MiscFlags = 0;
		TexDesc.SampleDesc.Count = 1;			// No anti-aliasing
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;

		if( !(_flags & CREATION_FLAGS::NO_TARGET) )
		{
			TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | (_flags & CREATION_FLAGS::TARGET_TEXTURE_VIEW ? D3D11_BIND_SHADER_RESOURCE : 0);
			TexDesc.Format = _format;

			ID3D11Texture2D* pTexture;
			if( _initalData )
			{
				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = _initalData;
				data.SysMemPitch = data.SysMemSlicePitch = 0;
				hr = Device::Device->CreateTexture2D( &TexDesc, &data, &pTexture );
			} else
				hr = Device::Device->CreateTexture2D( &TexDesc, nullptr, &pTexture );
			Assert( SUCCEEDED(hr) );

			// Now create the views
			hr = Device::Device->CreateRenderTargetView( pTexture, nullptr, &m_targetView );
			Assert( SUCCEEDED(hr) );
			if( _flags & CREATION_FLAGS::TARGET_TEXTURE_VIEW )
			{
				hr = Device::Device->CreateShaderResourceView( pTexture, nullptr, &m_textureView );
				Assert( SUCCEEDED(hr) );
			}
	
			// Return Views only - more is not needed
			pTexture->Release();
		}

		if( !(_flags & CREATION_FLAGS::NO_DEPTH) )
		{
			TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (_flags & CREATION_FLAGS::DEPTH_TEXTURE_VIEW ? D3D11_BIND_SHADER_RESOURCE : 0);
			TexDesc.Format = DXGI_FORMAT_R32_TYPELESS;//DXGI_FORMAT_R24G8_TYPELESS;//

			D3D11_SHADER_RESOURCE_VIEW_DESC ShaderDesc;
			memset( &ShaderDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
			ShaderDesc.Format = DXGI_FORMAT_R32_FLOAT;//DXGI_FORMAT_R24_UNORM_X8_TYPELESS;//
			ShaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			ShaderDesc.Texture2D.MostDetailedMip = 0;
			ShaderDesc.Texture2D.MipLevels = 1;

			D3D11_DEPTH_STENCIL_VIEW_DESC DepthDesc;
			DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_D24_UNORM_S8_UINT;//
			DepthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			DepthDesc.Texture2D.MipSlice = 0;
			DepthDesc.Flags = 0;

			ID3D11Texture2D* pTexture;
			hr = Device::Device->CreateTexture2D( &TexDesc, nullptr, &pTexture );
			Assert( SUCCEEDED(hr) );

			// Now create the views
			hr = Device::Device->CreateDepthStencilView( pTexture, &DepthDesc, &m_depthBufferRef );
			Assert( SUCCEEDED(hr) );
			if( _flags & CREATION_FLAGS::DEPTH_TEXTURE_VIEW )
			{
				hr = Device::Device->CreateShaderResourceView( pTexture, &ShaderDesc, &m_DSTextureView );
				Assert( SUCCEEDED(hr) );
			}

			pTexture->Release();
		}
	}

	// ********************************************************************* //
	RenderTarget::~RenderTarget()
	{
		delete m_viewPort;
		if( m_textureView ) m_textureView->Release();
		if( m_targetView ) m_targetView->Release();
		if( m_depthBufferRef ) m_depthBufferRef->Release();
		if( m_DSTextureView ) m_DSTextureView->Release();
	}


	// ********************************************************************* //
	// A copy constructor which uses the reference counting.
	/*RenderTarget::RenderTarget( const RenderTarget& _RT )
	{
		m_textureView = _RT.m_textureView;
		m_targetView = _RT.m_targetView;
		m_textureView->AddRef();
		m_targetView->AddRef();
	}*/

	
	// ********************************************************************* //
	// Set the texture to be read in the shader.
	void RenderTarget::SetAsTexture( uint _slot ) const
	{
		// To enable read of the target buffer use flag TARGET_TEXTURE_VIEW
		// during construction.
		Assert(m_textureView);

	//	ID3D11ShaderResourceView* const pNull = 0;
	//	if( m_lastTextureSlot != 0xffffffff && m_lastTextureSlot != _slot )
	//		Device::Context->PSSetShaderResources( m_lastTextureSlot, 1, &pNull );
		m_lastTextureSlot = _slot;
		Device::Context->PSSetShaderResources( _slot, 1, &m_textureView );
	}


	// ********************************************************************* //
	// Set the according depth buffer texture to be read in the shader.
	void RenderTarget::SetDSAsTexture( uint _slot ) const
	{
		// To enable read of the depth buffer use flag DEPTH_TEXTURE_VIEW
		// during construction.
		Assert( m_DSTextureView );

		m_DSLastTextureSlot = _slot;
		Device::Context->PSSetShaderResources( _slot, 1, &m_DSTextureView );
	}


	// ********************************************************************* //
	// Set as current target.
	void RenderTarget::SetAsTarget() const
	{
		// Disable read (not allowed to to both on the same time)
		if( m_lastTextureSlot != 0xffffffff )
		{
			ID3D11ShaderResourceView* const pNull = 0;
			Device::Context->PSSetShaderResources( m_lastTextureSlot, 1, &pNull );
			m_lastTextureSlot = 0xffffffff;
		}
		if( m_DSLastTextureSlot != 0xffffffff )
		{
			ID3D11ShaderResourceView* const pNull = 0;
			Device::Context->PSSetShaderResources( m_DSLastTextureSlot, 1, &pNull );
			m_DSLastTextureSlot = 0xffffffff;
		}
		Device::Context->OMSetRenderTargets( 1, &m_targetView, m_depthBufferRef );
		Device::Context->RSSetViewports( 1, m_viewPort );
	}

	// ********************************************************************* //
	// Overwrites this target with a constant color.
	void RenderTarget::Clear( const float* _color )
	{
		Device::Context->ClearRenderTargetView( m_targetView, _color );
	}


	// ********************************************************************* //
	// Clears the corresponding depth buffer.
	//void RenderTarget::ClearZ( float _depth )
	//{
	//	Device::Context->ClearDepthStencilView( m_depthBufferRef, D3D11_CLEAR_DEPTH, _depth, 0 );
	//}

	uint RenderTarget::GetWidth() const		{ return uint(m_viewPort->Width); }
	uint RenderTarget::GetHeight() const	{ return uint(m_viewPort->Height); }
} // namespace Graphic