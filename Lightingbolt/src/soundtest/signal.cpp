#include "signal.hpp"

namespace Soundtest
{
	int Signal::getLength(float _frequency)
	{
		return (int)(TT/_frequency);
	}

	char *Signal::getSine(float _frequency)
	{
		int length=getLength(_frequency);
		char *res=new char[length];
		for(int x=0; x<length; x++)
		{
			char signal=toChar((float)sin(x*2.0f*PI*_frequency/TT));
			res[x]=signal;
		}
		return res;
	}

	char *Signal::getSquare(float _frequency)
	{
		int length=getLength(_frequency);
		char *res=new char[length];
		for(int x=0; x<length; x++)
		{
			if(x<length/2)
			{
				res[x]=toChar(-1);
			}
			else
			{
				res[x]=toChar(1);
			}
		}
		return res;
	}

	char *Signal::getNoise(int _length)
	{
		char *res=new char[_length];
		for(int x=0; x<_length; x++)
		{
			res[x]=toChar(((rand()%100)/100.0f*2)-1);
		}
		return res;
	}

	char Signal::toChar(float _signal)
	{
		return (char)(_signal*127+128);
	}
}