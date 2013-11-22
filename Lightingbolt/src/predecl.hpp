#pragma once

typedef unsigned int uint;
typedef struct HWND__* HWND;


struct DXGI_SWAP_CHAIN_DESC;
struct IDXGISwapChain;
struct DXGI_SAMPLE_DESC;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Graphic {
	class DX11Window;

	namespace Device {
		extern ID3D11Device* Device;
		extern ID3D11DeviceContext* Context;
		extern DX11Window* Window;
	};
} // namespace Graphic


namespace Sound {
	class Sound;
	class Source;
	class Device;
} // namespace Sound

namespace Math {
	class Vec2;
	class Vec3;
} // namespace Math

namespace Map {
	class Map;
} // namespace Map

namespace Utilities {
	class DynArray;
} // namespace Utilities

namespace GameStates {
	class IGameState;
	class Menu;
} // namespace GameStates

#ifdef _DEBUG
#include <intrin.h>

// Simple assertion like cassert, but stops in the line of source, where the
// assertion is located in not in some other file like the std::assert.
#define Assert(a) if(!(a)) __debugbreak()

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) if((A)!=(Const)) __debugbreak()
#define AssertNeq(A,Const) if((A)==(Const)) __debugbreak()

#else	// _DEBUG


#define Assert(a)

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) A
#define AssertNeq(A,Const) A

#endif // _DEBUG
