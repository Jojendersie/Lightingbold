#include "graphic/device.hpp"

#include <Windows.h>
#include <cstdint>

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include "gamestates/menu.hpp"

int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// *** INITIALIZATION ************************************************** //
	Graphic::DX11Window* window = new Graphic::DX11Window( 1024, 768, false );
	window->OnMouseMove = GameStates::MenuMouseMove;
	window->OnKeyDown = GameStates::MenuKeyDown;
	window->OnKeyUp = GameStates::MenuKeyUp;
	window->OnScroll = GameStates::MenuScroll;

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
				//pGame->Update( dTime, Saga::min(0.05, dDeltaTime) );
				//pGame->RenderFrame( dTime, Saga::min(0.05, dDeltaTime) );
			}
        }
    }

	// *** SHUTDOWN ******************************************************** //
	delete window;
	return 0;
}