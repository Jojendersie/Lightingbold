#include <iostream>

#include <vector>
using namespace std;

#include "../sound/sound.hpp"

namespace Soundtest
{
	class Melody
	{
	public:
		Melody(Sound::Sound sound, float bpm, vector<int> notes);
		~Melody();

		void update(double _deltaTime);
		void setPitch();
	private:
		Sound::Source *m_source;
		double m_curTime;
		float m_spb;
		vector<int> m_notes;
		int m_index;
	};
}