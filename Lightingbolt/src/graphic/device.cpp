#include <cstdint>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "device.hpp"
#include "../graphic/VertexBuffer.hpp"

static int g_iLastX;
static int g_iLastY;

namespace Graphic {

	namespace Device {
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;
		DX11Window* Window;
	};
	
	bool DX11Window::IsMultisampleEnable() const	{ return m_swapChainDesc->SampleDesc.Count>1; }
	const DXGI_SAMPLE_DESC& DX11Window::GetSampleDesc() const	{ return m_swapChainDesc->SampleDesc; }
	void DX11Window::Present() const				{ m_swapChain->Present( 0, 0 );}


	// Called every time the application receives a message
	LRESULT CALLBACK MessageProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch( message )
		{
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					BeginPaint( hwnd, &ps );
					EndPaint( hwnd, &ps );
					return 0;
				}
			case WM_MOUSEMOVE:
				{
					int iX = int16_t(lParam & 0xffff)	+ Device::Window->X(); 
					int iY = int16_t(lParam >> 16)		+ Device::Window->Y();
					int iDX = iX - g_iLastX;
					int iDY = iY - g_iLastY;

					// Call a game input method with the delta x,y
					Device::Window->OnMouseMove( iDX, iDY );

					// Capture the mouse at the window center
					int iCenterX = Device::Window->X() + Device::Window->Width()/2;
					int iCenterY = Device::Window->Y() + Device::Window->Height()/2;
					if( abs(iX - iCenterX) > 25 || abs(iY- iCenterY) > 25 )
					{
			//			SetCursorPos( iCenterX, iCenterY );
						g_iLastX = iCenterX;
						g_iLastY = iCenterY;
					} else {
						g_iLastX = iX;
						g_iLastY = iY;
					}

	//				char acBuf[128];
		//			sprintf( acBuf, "%d %d\n", iX, iY );
			//		OutputDebugString( acBuf );
					return 0;
				}
			case WM_LBUTTONDOWN:
				Device::Window->OnKeyDown( VK_LBUTTON );
				return 0;
			case WM_RBUTTONDOWN:
				Device::Window->OnKeyDown( VK_RBUTTON );
				return 0;
			case WM_KEYDOWN:
				if( !((lParam >> 30) & 1) )
					Device::Window->OnKeyDown( int(wParam) );
				return 0;
			case WM_LBUTTONUP:
				Device::Window->OnKeyUp( VK_LBUTTON );
				return 0;
			case WM_RBUTTONUP:
				Device::Window->OnKeyUp( VK_RBUTTON );
				return 0;
			case WM_KEYUP:
				Device::Window->OnKeyUp( int(wParam) );
				return 0;
			case WM_MOUSEWHEEL:
				Device::Window->OnScroll( int16_t(wParam>>16) );
				return 0;
			case WM_SIZE: // Window size has been changed
				// TODO: Resize D3D render target
				return 0;
			case WM_CHAR:
				if (wParam == VK_ESCAPE)
					PostQuitMessage(0);
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			default:
				return 1;//DefWindowProc(m_HWND, message, wParam, lParam);
		}
	}





	DX11Window::DX11Window( int _width, int _height, bool _fullScreen ) :
		m_fullScreen( _fullScreen ),
		m_device( nullptr ),
		m_devContext( nullptr ),
		m_swapChain( nullptr )
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle( nullptr );
		// Register our window class
		WNDCLASSEX WinClass = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MessageProc,
							0, 0, hInstance, nullptr, nullptr, nullptr, nullptr, "DX11Window" };
		RegisterClassEx( &WinClass );

		// Create a window
		RECT Rc = { 0, 0, _width, _height };
		AdjustWindowRect( &Rc, WS_OVERLAPPEDWINDOW, FALSE );
		m_width = Rc.right-Rc.left;
		m_height = Rc.bottom-Rc.top;
		// TODO is das mit den Borders so richtig, oder sollte die größe für Rendertargets anders sein?
		m_HWND = CreateWindow( "DX11Window", "Sagaramatha",
							   WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
							   m_width, m_height, nullptr, nullptr, hInstance, nullptr );

		// Something with the windows window creation went wrong
		Assert( m_HWND );

		// Show window without cursor
		ShowWindow( m_HWND, SW_SHOW );
		UpdateWindow( m_HWND );
		ShowCursor( TRUE );

		// Catch the cursur
		GetWindowRect( m_HWND, &Rc );
		m_posX = Rc.left;
		m_posY = Rc.top;
	//	ClipCursor( &Rc );

		int iCenterX = X() + Width()/2;
		int iCenterY = Y() + Height()/2;
	//	SetCursorPos( iCenterX, iCenterY );
		g_iLastX = iCenterX;
		g_iLastY = iCenterY;

		Device::Window = this;

		InitDevice();

		m_screenQuad = new Graphic::VertexBuffer(sizeof(Vertex), 1);
		Vertex quad;
		quad.Position = 0.0f;
		quad.Size = 1.0f;
		m_screenQuad->upload(&quad, 1);
	}

	DX11Window::~DX11Window()
	{
		Device::Window = nullptr;
		Device::Device = nullptr;
		Device::Context = nullptr;

		if( m_depthStencilState ) m_depthStencilState->Release();
		if( m_blendStateAlpha ) m_blendStateAlpha->Release();
		if( m_blendStateAdd ) m_blendStateAdd->Release();
		if( m_blendStateMult ) m_blendStateMult->Release();
		if( m_rasterState ) m_rasterState->Release();
		if( m_linearSampler ) m_linearSampler->Release();

		delete m_screenQuad;

		if( m_devContext ) 
			m_devContext->ClearState();

		if( m_swapChain ) while( 0 != m_swapChain->Release() );
		if( m_devContext ) while( 0 != m_devContext->Release() );
#if defined(DEBUG) | defined(_DEBUG) // Quick and dirty - avoid release crash!!!
		if( m_device ) while( 0 != m_device->Release() );
#endif
		delete m_swapChainDesc;
	}


	void DX11Window::InitDevice()
	{
		HRESULT hr = S_OK;

		// Create D3D11 device and swap chain
		unsigned uiCreateDeviceFlags = 0;
		#ifdef _DEBUG
			uiCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		m_swapChainDesc = new DXGI_SWAP_CHAIN_DESC;
		memset( m_swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC) );
		m_swapChainDesc->BufferCount = 1;
		m_swapChainDesc->BufferDesc.Width = m_width;
		m_swapChainDesc->BufferDesc.Height = m_height;
		m_swapChainDesc->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swapChainDesc->BufferDesc.RefreshRate.Numerator = 0;
		m_swapChainDesc->BufferDesc.RefreshRate.Denominator = 1;
		m_swapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapChainDesc->OutputWindow = m_HWND;
		// Enable multisampling
		m_swapChainDesc->SampleDesc.Count = 1;
		m_swapChainDesc->SampleDesc.Quality = 0;
		m_swapChainDesc->Windowed = !m_fullScreen;
		m_swapChainDesc->Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		auto FeatureLvl = D3D_FEATURE_LEVEL_10_1;
		// Try to create a hardware accelerated device with multisample antialiasing first
		// TODO: reduce this 3 calls to safe space
		hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uiCreateDeviceFlags, 
											&FeatureLvl, 1, D3D11_SDK_VERSION, m_swapChainDesc, &m_swapChain, 
											&m_device, nullptr, &m_devContext );
		if( FAILED(hr) )
		{
			// If failed, try to create a reference device
			hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr, uiCreateDeviceFlags, 
												&FeatureLvl, 1, D3D11_SDK_VERSION, m_swapChainDesc, &m_swapChain, 
												&m_device, nullptr, &m_devContext);

			// Your program ends here ... no d3d device found
			Assert( SUCCEEDED(hr) );
		}

		Device::Device = m_device;
		Device::Context = m_devContext;

		// Setup stages
		m_devContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

		CreateStates();
	}


	// ********************************************************************* //
	void DX11Window::CreateStates()
	{
		HRESULT hr;

		// No Z-Test
		D3D11_DEPTH_STENCILOP_DESC od;
		od.StencilFunc = D3D11_COMPARISON_ALWAYS;
		od.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		od.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		od.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
		DepthStencilDesc.DepthEnable = false;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		DepthStencilDesc.StencilEnable = true;
		DepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		DepthStencilDesc.FrontFace = od;
		DepthStencilDesc.BackFace = od;
		hr = Device::Device->CreateDepthStencilState( &DepthStencilDesc, &m_depthStencilState );
		Assert( SUCCEEDED( hr ) );

		// Standard alpha blending
		D3D11_BLEND_DESC BlendDesc;
		BlendDesc.AlphaToCoverageEnable = false;
		BlendDesc.IndependentBlendEnable = false;
		BlendDesc.RenderTarget[0].BlendEnable = true;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = Device::Device->CreateBlendState( &BlendDesc, &m_blendStateAlpha );
		Assert( SUCCEEDED( hr ) );
		// Additive
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		hr = Device::Device->CreateBlendState( &BlendDesc, &m_blendStateAdd );
		Assert( SUCCEEDED( hr ) );
		// Negative multiplication
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		hr = Device::Device->CreateBlendState( &BlendDesc, &m_blendStateMult );
		Assert( SUCCEEDED( hr ) );

		// Solid fill mode no culling
		D3D11_RASTERIZER_DESC RasterDesc;
		memset( &RasterDesc, 0, sizeof(RasterDesc) );
		RasterDesc.MultisampleEnable = false;
		RasterDesc.FillMode = D3D11_FILL_SOLID;
		RasterDesc.CullMode = D3D11_CULL_NONE;
		hr = Device::Device->CreateRasterizerState( &RasterDesc, &m_rasterState );
		Assert( SUCCEEDED( hr ) );


		// Create a texture sampler state description.
		D3D11_SAMPLER_DESC SamplerDesc;
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 1;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamplerDesc.BorderColor[0] = 0;
		SamplerDesc.BorderColor[1] = 0;
		SamplerDesc.BorderColor[2] = 0;
		SamplerDesc.BorderColor[3] = 0;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Device::Device->CreateSamplerState( &SamplerDesc, &m_linearSampler );
		Assert( SUCCEEDED(hr) );

		// Set them all
		Device::Context->OMSetBlendState( m_blendStateAlpha, nullptr, 0xffffffff );
		Device::Context->RSSetState( m_rasterState );
		Device::Context->OMSetDepthStencilState( m_depthStencilState, 0 );
		Device::Context->PSSetSamplers( 0, 1, &m_linearSampler );
		Device::Context->GSSetSamplers( 0, 1, &m_linearSampler );
	}


	void DX11Window::setBlendMode( BLEND_MODES _mode )
	{
		if( _mode == BLEND_MODES::ALPHA )
			Device::Context->OMSetBlendState( m_blendStateAlpha, nullptr, 0xffffffff );
		else if( _mode == BLEND_MODES::ADDITIVE )
			Device::Context->OMSetBlendState( m_blendStateAdd, nullptr, 0xffffffff );
		else Device::Context->OMSetBlendState( m_blendStateMult, nullptr, 0xffffffff );
	}

	void DX11Window::drawScreenQuad()
	{
		Graphic::Vertex::SetLayout();
		m_screenQuad->set();
		Graphic::Device::Context->Draw( 1, 0 );
	}

} // namespace Graphic
