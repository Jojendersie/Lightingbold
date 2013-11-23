#include "note.hpp"

namespace Soundtest
{
	int Note::getLength(float frequency)
	{
		return (int)(TT/frequency);
	}

	char *Note::getSine(float frequency)
	{
		int length=getLength(frequency);
		char *res=new char[length];
		for(int x=0; x<length; x++)
		{
			char signal=toChar((float)sin(x*2.0f*PI*frequency/TT));
			res[x]=signal;
		}
		return res;
	}

	char *Note::getSquare(float frequency)
	{
		int length=getLength(frequency);
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

	char *Note::getNoise(int length)
	{
		char *res=new char[length];
		for(int x=0; x<length; x++)
		{
			res[x]=toChar(((rand()%100)/100.0f*2)-1);
		}
		return res;
	}

	char Note::toChar(float note)
	{
		return (char)(note*127+128);
	}
}