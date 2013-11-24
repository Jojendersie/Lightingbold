#pragma once

#include "../utilities/smalldynarray.hpp"
using namespace Utilities;

#include "melody.hpp"
#include "note.hpp"

namespace Soundtest
{
	class Song
	{
	public:
		static DynArray<Note> twosonHigh();
		static DynArray<Note> twosonLow();
	};
}