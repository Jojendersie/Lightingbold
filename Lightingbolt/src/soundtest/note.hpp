#include <vector>
using namespace std;

namespace Soundtest
{
	class Note
	{
	public:
		static vector<float> getNote(float frequency, int length);
		static vector<float> getNoise(int length); //srand has to be called before this
		static char toChar(float note);
	};
}