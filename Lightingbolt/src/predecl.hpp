#pragma once

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