#pragma once

typedef unsigned int uint;

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