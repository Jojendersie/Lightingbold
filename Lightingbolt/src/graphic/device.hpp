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

		/// \brief Create all state objects required for the application
		void CreateStates();
		ID3D11DepthStencilState* m_depthStencilState;	///< No Z-Test
		ID3D11BlendState* m_blendStateAlpha;			///< Standard alpha blending
		ID3D11BlendState* m_blendStateAdd;				///< Pure additive blending
		ID3D11BlendState* m_blendStateMult;				///< Inverse multiplicative with color
		ID3D11RasterizerState* m_rasterState;			///< Solid fill mode no culling
		ID3D11SamplerState* m_linearSampler;				///< Point sampler with clamp

		Graphic::VertexBuffer* m_screenQuad;			///< One vertex buffer to render screen aligned quads.
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

		enum struct BLEND_MODES
		{
			ALPHA,
			ADDITIVE,
			INV_MULT
		};
		void setBlendMode( BLEND_MODES _mode );

		void drawScreenQuad();
	};

} // namespace Graphic