#include <cstdint>
#include <cassert>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "device.hpp"

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
						SetCursorPos( iCenterX, iCenterY );
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
		assert( m_HWND );

		// Show window without cursor
		ShowWindow( m_HWND, SW_SHOW );
		UpdateWindow( m_HWND );
		ShowCursor( FALSE );

		// Catch the cursur
		GetWindowRect( m_HWND, &Rc );
		m_posX = Rc.left;
		m_posY = Rc.top;
		ClipCursor( &Rc );

		int iCenterX = X() + Width()/2;
		int iCenterY = Y() + Height()/2;
		SetCursorPos( iCenterX, iCenterY );
		g_iLastX = iCenterX;
		g_iLastY = iCenterY;

		Device::Window = this;

		InitDevice();
	}

	DX11Window::~DX11Window()
	{
		Device::Window = nullptr;
		Device::Device = nullptr;
		Device::Context = nullptr;

		if( m_devContext ) 
			m_devContext->ClearState();

		if( m_swapChain ) while( 0 != m_swapChain->Release() );
		if( m_devContext ) while( 0 != m_devContext->Release() );
		if( m_device ) while( 0 != m_device->Release() );
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
		m_swapChainDesc->SampleDesc.Count = 4;
		m_swapChainDesc->SampleDesc.Quality = 0;
		m_swapChainDesc->Windowed = !m_fullScreen;
		m_swapChainDesc->Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		auto FeatureLvl = D3D_FEATURE_LEVEL_11_0;
		// Try to create a hardware accelerated device with multisample antialiasing first
		// TODO: reduce this 3 calls to safe space
		hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uiCreateDeviceFlags, 
											&FeatureLvl, 1, D3D11_SDK_VERSION, m_swapChainDesc, &m_swapChain, 
											&m_device, nullptr, &m_devContext );
		if( FAILED(hr) )
		{
			// If failed, try without antialiasing
			m_swapChainDesc->SampleDesc.Count = 1;
			hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uiCreateDeviceFlags, 
												&FeatureLvl, 1, D3D11_SDK_VERSION, m_swapChainDesc, &m_swapChain, 
												&m_device, nullptr, &m_devContext);
			if( FAILED(hr) )
			{
				// If failed, try to create a reference device
				hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr, uiCreateDeviceFlags, 
													&FeatureLvl, 1, D3D11_SDK_VERSION, m_swapChainDesc, &m_swapChain, 
													&m_device, nullptr, &m_devContext);

				// Your program ends here ... no d3d device found
				assert( SUCCEEDED(hr) );
			}
		}

		Device::Device = m_device;
		Device::Context = m_devContext;

		// Setup stages
		// TODO: reicht das hier?
		m_devContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	}


	// ************************************************************************** //
	// Mapped method for content creation
	ID3D11Buffer* DX11Window::CreateStaticStdBuffer( unsigned _size, const void* _data, unsigned _bindFlag ) const
	{
		D3D11_BUFFER_DESC Desc;
		Desc.BindFlags = _bindFlag;
		Desc.ByteWidth = _size;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;
		Desc.StructureByteStride = 0;//?
		Desc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA Data;
		Data.pSysMem = _data;
		Data.SysMemPitch = 0;
		Data.SysMemSlicePitch = 0;

		ID3D11Buffer* pResult;
		HRESULT hr = m_device->CreateBuffer( &Desc, &Data, &pResult );
		assert( SUCCEEDED(hr) );

		return pResult;
	}

} // namespace Graphic
