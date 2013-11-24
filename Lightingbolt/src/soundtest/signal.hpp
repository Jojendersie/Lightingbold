#pragma once

#include <cmath>
#include <cstdlib>

#define TT 20000
#define PI 3.141592653

namespace Soundtest
{
	class Signal
	{
	public:
		static int getLength(float _frequency);
		static char *getSine(float _frequency);
		static char *getSquare(float _frequency);
		static char *getNoise(int _length); //srand has to be called before this
		static char toChar(float _signal);
	};
}