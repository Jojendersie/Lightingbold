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
#include "gamestates/Ingame.hpp"
#include "graphic/device.hpp"
#include "graphic/RenderTarget.hpp"
#include "graphic/Shader.hpp"
#include "graphic/Vertex.hpp"
#include "graphic/UniformBuffer.hpp"
#include "generator\Random.hpp"

#include "soundtest/soundmanager.hpp"

#if !defined(DEBUG) && !defined(_DEBUG)
#include "shader/Blob.h"
#include "shader/Blur.h"
#include "shader/InitPhotons.h"
#include "shader/PassPhoton.h"
#include "shader/Photon.h"
#include "shader/Quad.h"
#include "shader/Quad_gs.h"
#include "shader/RefractionMap.h"
#include "shader/SimPhotons.h"
#include "shader/Textured.h"
#endif

// *** FUNCTION DECLARATIONS *********************************************** //
void CreateRenderTargets();
void CreateShaders();
#ifdef DYNAMIC_SHADER_RELOAD
void ReloadShaders();
#endif

// *** GLOBAL STUFF main.cpp IS MORE OR LESS THE GAME CLASS **************** //
GameStates::Menu* g_StateMenu;
GameStates::Ingame* g_StateIngame;
GameStates::IGameState* g_State;
Generators::Random* g_rand;
Graphic::RenderTargetList* g_RenderTargets;
Graphic::ShaderList* g_ShaderList;
Graphic::UniformBuffer* g_ShaderConstants;
void MouseMove(int _dx, int _dy)	{ g_State->MouseMove(_dx,_dy); }
void KeyDown(int _key)				{ g_State->KeyDown(_key); }
void KeyUp(int _key)				{ g_State->KeyUp(_key); }
void Scroll(int _delta)				{ g_State->Scroll(_delta); }



int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//	_CrtSetBreakAlloc(159);
#endif

	Soundtest::SoundManager sound;

	// *** INITIALIZATION ************************************************** //
#if defined(DEBUG) | defined(_DEBUG)
	Graphic::DX11Window* window = new Graphic::DX11Window( 1024, 768, false );
#else
	Graphic::DX11Window* window = new Graphic::DX11Window( 1366, 768, true );
#endif
	g_rand = new Generators::Random(0);
	g_StateMenu = new GameStates::Menu;
	g_StateIngame = new GameStates::Ingame;
	g_State = g_StateIngame;
	g_ShaderConstants = new Graphic::UniformBuffer();
	window->OnMouseMove = MouseMove;
	window->OnKeyDown = KeyDown;
	window->OnKeyUp = KeyUp;
	window->OnScroll = Scroll;
	CreateRenderTargets();
	CreateShaders();

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

				// Call update stuff
#ifdef DYNAMIC_SHADER_RELOAD
				ReloadShaders();
#endif
				g_State->Render(dTime, dDeltaTime, *g_RenderTargets, *g_ShaderList, g_ShaderConstants);
				g_State->Update(dTime, dDeltaTime);

				sound.update(dDeltaTime);
			}
        }
    }

	// *** SHUTDOWN ******************************************************** //
	delete g_RenderTargets;
	delete g_ShaderList;
	delete g_StateMenu;
	delete g_ShaderConstants;
	Graphic::Vertex::ReleaseLayout();
	Graphic::PhotonVertex::releaseLayout();
	delete g_StateIngame;
	delete window;
	delete g_rand;
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

void CreateShaders()
{
	g_ShaderList = new Graphic::ShaderList;

#if defined( DEBUG ) || defined( _DEBUG )
	g_ShaderList->VSPassThrough = new Graphic::Shader(  L"shader/Quad.vs", Graphic::Shader::Type::VERTEX );
	Graphic::Vertex::InitLayout( g_ShaderList->VSPassThrough );
	g_ShaderList->VSPassPhoton = new Graphic::Shader(  L"shader/PassPhoton.vs", Graphic::Shader::Type::VERTEX );
	Graphic::PhotonVertex::initLayout( g_ShaderList->VSPassPhoton );

	g_ShaderList->GSQuad = new Graphic::Shader(  L"shader/Quad.gs", Graphic::Shader::Type::GEOMETRY );
	g_ShaderList->GSInitPhotons = new Graphic::Shader(  L"shader/InitPhotons.gs", Graphic::Shader::Type::GEOMETRY,
		Graphic::PhotonVertex::getOutputLayoutDesc(), Graphic::PhotonVertex::getOutputLayoutNum() );
	g_ShaderList->GSSimulate = new Graphic::Shader(  L"shader/SimPhotons.gs", Graphic::Shader::Type::GEOMETRY,
		Graphic::PhotonVertex::getOutputLayoutDesc(), Graphic::PhotonVertex::getOutputLayoutNum() );

	g_ShaderList->PSBlob = new Graphic::Shader(  L"shader/Blob.ps", Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSPhoton = new Graphic::Shader(  L"shader/Photon.ps", Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSTexture = new Graphic::Shader(  L"shader/Textured.ps", Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSRefractionMap = new Graphic::Shader(  L"shader/RefractionMap.ps", Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSBlur = new Graphic::Shader(  L"shader/Blur.ps", Graphic::Shader::Type::PIXEL );
#else
	g_ShaderList->VSPassThrough = new Graphic::Shader(  Quad_vs, sizeof(Quad_vs), Graphic::Shader::Type::VERTEX );
	Graphic::Vertex::InitLayout( g_ShaderList->VSPassThrough );
	g_ShaderList->VSPassPhoton = new Graphic::Shader(  PassPhoton_vs, sizeof(PassPhoton_vs), Graphic::Shader::Type::VERTEX );
	Graphic::PhotonVertex::initLayout( g_ShaderList->VSPassPhoton );

	g_ShaderList->GSQuad = new Graphic::Shader(  Quad_gs, sizeof(Quad_gs), Graphic::Shader::Type::GEOMETRY );
	g_ShaderList->GSInitPhotons = new Graphic::Shader(  InitPhotons_gs, sizeof(InitPhotons_gs), Graphic::Shader::Type::GEOMETRY,
		Graphic::PhotonVertex::getOutputLayoutDesc(), Graphic::PhotonVertex::getOutputLayoutNum() );
	g_ShaderList->GSSimulate = new Graphic::Shader(  SimPhotons_gs, sizeof(SimPhotons_gs), Graphic::Shader::Type::GEOMETRY,
		Graphic::PhotonVertex::getOutputLayoutDesc(), Graphic::PhotonVertex::getOutputLayoutNum() );

	g_ShaderList->PSBlob = new Graphic::Shader(  Blob_ps, sizeof(Blob_ps), Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSPhoton = new Graphic::Shader(  Photon_ps, sizeof(Photon_ps), Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSTexture = new Graphic::Shader(  Textured_ps, sizeof(Textured_ps), Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSRefractionMap = new Graphic::Shader(  RefractionMap_ps, sizeof(RefractionMap_ps), Graphic::Shader::Type::PIXEL );
	g_ShaderList->PSBlur = new Graphic::Shader(  Blur_ps, sizeof(Blur_ps), Graphic::Shader::Type::PIXEL );
#endif
}

#ifdef DYNAMIC_SHADER_RELOAD
void ReloadShaders()
{
	g_ShaderList->VSPassThrough->dynamicReload();
	g_ShaderList->VSPassPhoton->dynamicReload();

	g_ShaderList->GSQuad->dynamicReload();
	g_ShaderList->GSInitPhotons->dynamicReload();
	g_ShaderList->GSSimulate->dynamicReload();

	g_ShaderList->PSBlob->dynamicReload();
	g_ShaderList->PSPhoton->dynamicReload();
	g_ShaderList->PSTexture->dynamicReload();
	g_ShaderList->PSRefractionMap->dynamicReload();
	g_ShaderList->PSBlur->dynamicReload();
}
#endif



void SwitchGameState( GameStates::GS _State )
{
	switch(_State)
	{
		case GameStates::GS::INGAME: 
			delete g_StateIngame;
			g_StateIngame = new GameStates::Ingame;
			
			g_State = g_StateIngame; break;
		case GameStates::GS::MENU: g_State = g_StateMenu; break;
	}
}