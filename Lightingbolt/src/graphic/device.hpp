#pragma once

#include "../predecl.hpp"

namespace Graphic {
	// The DX11-Device and Window.
	class DX11Window
	{
	private:
		HWND m_HWND;
		DXGI_SWAP_CHAIN_DESC*	m_swapChainDesc;
		IDXGISwapChain*			m_swapChain;

		int m_width;
		int m_height;
		int m_posX;
		int m_posY;
		bool m_fullScreen;

		void InitDevice();

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_devContext;
	public:
		DX11Window( int _width, int _height, bool _fullScreen );
		~DX11Window();

		void (*OnMouseMove)(int,int);
		void (*OnKeyDown)(int);
		void (*OnKeyUp)(int);
		void (*OnScroll)(int);


		int Width() const					{ return m_width; }
		int Height() const					{ return m_height; }
		int X() const						{ return m_posX; }
		int Y() const						{ return m_posY; }
		IDXGISwapChain* GetSwapChain() const{ return m_swapChain; }
		bool IsMultisampleEnable() const;
		const DXGI_SAMPLE_DESC& GetSampleDesc() const;

		void Present() const;

		/// \brief Create a static (immutable) vertex or indexbuffer.
		/// \param [in] _size Size of the bufferdata in bytes.
		/// \param [in] _data Memory with the data. After the creation the
		///		data inside the created buffer cannot be changed so _data
		///		should contain the final result.
		/// \param [in] _bindFlag D3D11_BIND_VERTEX_BUFFER or D3D11_BIND_INDEX_BUFFER
		ID3D11Buffer* CreateStaticStdBuffer( unsigned _size, const void* _data, unsigned _bindFlag ) const;
	};

} // namespace Graphic