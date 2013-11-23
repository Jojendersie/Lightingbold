#pragma once

#include <iostream>
using namespace std;

#include "../sound/sound.hpp"

namespace Soundtest
{
	class Melody
	{
	public:
		Melody(Sound::Sound sound, float bpm, int *notes, int noteSize);
		~Melody();

		void update(double _deltaTime);
		void setPitch();
	private:
		Sound::Source *m_source;
		double m_curTime;
		float m_spb;
		int *m_notes;
		int m_noteSize;
		int m_index;
	};
}