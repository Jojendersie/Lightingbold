#include "note.hpp"

namespace Soundtest
{
	vector<float> Note::getNote(float frequency, int length)
	{
		vector<float> res;
		for (int i=0; i<length; i++)
		{
			res.push_back(sin(i*6.283185307*(frequency)/20000.0));
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