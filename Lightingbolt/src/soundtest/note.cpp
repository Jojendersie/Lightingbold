#include "note.hpp"

namespace Soundtest
{
	vector<float> Note::getSine(float frequency, int length)
	{
		float pi=3.141592653;
		float tt=20000.0f;

		vector<float> res;
		for (int x=0; x<length; x++)
		{
			res.push_back(sin(x*2.0f*pi*frequency/tt));
		}
		return res;
	}

	vector<float> Note::getSine(float frequency)
	{
		float pi=3.141592653;
		float tt=20000.0f;

		vector<float> res;
		int length=tt/frequency;
		for(int x=0; x<length; x++)
		{
			res.push_back(sin(x*2.0f*pi*frequency/tt));
		}
		return res;
	}

	vector<float> Note::getSquare(float frequency)
	{
		float tt=20000.0f;

		vector<float> res;
		int length=tt/frequency;
		for(int x=0; x<length; x++)
		{
			if(x<length/2)
			{
				res.push_back(-1);
			}
			else
			{
				res.push_back(1);
			}
		}
		return res;
	}

	vector<float> Note::getNoise(int length)
	{
		vector<float> res;
		for(int i=0; i<length; i++)
		{
			res.push_back(((rand()%100)/100.0f*2)-1);
		}
		return res;
	}

	char Note::toChar(float note)
	{
		return (char)(note*127+128);
	}
}