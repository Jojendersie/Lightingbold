#include "note.hpp"

namespace Soundtest
{
	std::string Note::getNote(float frequency, int length)
	{
		std::string pcBuffer;
		for (int i=0; i < length; i++)
		{
			double x;
			x = sin(i*6.283185307*(frequency)/20000.0);
			pcBuffer.push_back((char)(127*x+128));
		}
		return pcBuffer;
	}
}