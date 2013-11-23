#include <vector>
using namespace std;

namespace Soundtest
{
	class Note
	{
	public:
		static vector<float> getSine(float frequency, int length);
		static vector<float> getSine(float frequency);
		static vector<float> getSquare(float frequency);
		static vector<float> getNoise(int length); //srand has to be called before this
		static char toChar(float note);
	};
}