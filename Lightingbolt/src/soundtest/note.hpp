#pragma once

#include <cmath>
#include <cstdlib>

#define TT 20000
#define PI 3.141592653

namespace Soundtest
{
	class Note
	{
	public:
		static int getLength(float frequency);
		static char *getSine(float frequency);
		static char *getSquare(float frequency);
		static char *getNoise(int length); //srand has to be called before this
		static char toChar(float note);
	};
}