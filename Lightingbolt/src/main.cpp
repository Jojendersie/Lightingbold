#include <Windows.h>
#include <cstdint>
#include <d3d11.h>

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include "predecl.hpp"
#include "gamestates/menu.hpp"
#include "graphic/device.hpp"
#include "graphic/RenderTarget.hpp"
#include "graphic/Shader.hpp"

// *** FUNCTION DECLARATIONS *********************************************** //
void CreateRenderTargets();

// *** GLOBAL STUFF main.cpp IS MORE OR LESS THE GAME CLASS **************** //
GameStates::IGameState* g_State;
void MouseMove(int _dx, int _dy)	{ g_State->MouseMove(_dx,_dy); }
void KeyDown(int _key)				{ g_State->KeyDown(_key); }
void KeyUp(int _key)				{ g_State->KeyUp(_key); }
void Scroll(int _delta)				{ g_State->Scroll(_delta); }
Graphic::RenderTargetList* g_RenderTargets;



int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// *** INITIALIZATION ************************************************** //
	Graphic::DX11Window* window = new Graphic::DX11Window( 1024, 768, false );
	g_State = new GameStates::Menu;
	window->OnMouseMove = MouseMove;
	window->OnKeyDown = KeyDown;
	window->OnKeyUp = KeyUp;
	window->OnScroll = Scroll;
	CreateRenderTargets();

	Graphic::Shader Test( L"shader/QuadShader.vs", Graphic::Shader::Type::VERTEX );

	double dTime = 0.0;
	uint64_t uiOldTime, uiNewTime;
	QueryPerformanceFrequency( (LARGE_INTEGER*)&uiNewTime );
	QueryPerformanceCounter( (LARGE_INTEGER*)&uiOldTime );
	double dInvFreq = 1.0/uiNewTime;

	// *** MAIN LOOP ******************************************************* //
    MSG Msg = {0};
    while( WM_QUIT != Msg.message )
    {
        if( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &Msg );
            DispatchMessage( &Msg );
        }
        else
        {
			// Calculate time scince last frame
			QueryPerformanceCounter( (LARGE_INTEGER*)&uiNewTime );
			double dDeltaTime = ( uiNewTime - uiOldTime ) * dInvFreq;
			// Limiting to 120 fps
			if( dDeltaTime > 0.008333333333 )
			{
				uiOldTime = uiNewTime;
				dTime += dDeltaTime;

				// Call stuff
				g_State->Render(dTime, dDeltaTime, *g_RenderTargets);
				//pGame->Update( dTime, Saga::min(0.05, dDeltaTime) );
				//pGame->RenderFrame( dTime, Saga::min(0.05, dDeltaTime) );
			}
        }
    }

	// *** SHUTDOWN ******************************************************** //
	delete g_RenderTargets;
	delete g_State;
	delete window;
	return 0;
}



void CreateRenderTargets()
{
	g_RenderTargets = new Graphic::RenderTargetList;
	HRESULT hr;

    // Create the backbuffer render target and depth-stencil view
    ID3D11Texture2D* txBackBuffer = nullptr;
	hr = Graphic::Device::Window->GetSwapChain()->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&txBackBuffer );
	Assert( SUCCEEDED(hr) );

	ID3D11RenderTargetView* pBackbufferView;
	hr = Graphic::Device::Device->CreateRenderTargetView( txBackBuffer, nullptr, &pBackbufferView );
	Assert( SUCCEEDED(hr) );

    txBackBuffer->Release();

	// TODO: due to post process the backbuffer does not need a depth-stencil buffer
	g_RenderTargets->BackBuffer = new Graphic::RenderTarget( pBackbufferView, nullptr, Graphic::RenderTarget::CREATION_FLAGS::NO_DEPTH );
}