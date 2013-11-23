#pragma once

typedef unsigned int uint;
typedef struct HWND__* HWND;
typedef enum DXGI_FORMAT;


struct DXGI_SWAP_CHAIN_DESC;
struct IDXGISwapChain;
struct DXGI_SAMPLE_DESC;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Graphic {
	class DX11Window;
	class RenderTarget;
	struct RenderTargetList;
	class Shader;
	struct Vertex;
	struct ShaderList;

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

namespace Ai {
	class GameObject;
} // namespace Ai

namespace GameStates {
	class IGameState;
	class Menu;
	class Ingame;
	enum struct GS {
		MENU,
		INGAME
	};
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


const float CLEAR_COLOR[4] = {1.0f, 0.564705882f, 0.003921569f, 1.0f };

void SwitchGameState( GameStates::GS _State );