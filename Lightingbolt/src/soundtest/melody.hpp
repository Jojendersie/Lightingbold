#pragma once

#include <iostream>
using namespace std;

#include "../utilities/smalldynarray.hpp"
using namespace Utilities;

#include "../sound/sound.hpp"
#include "../soundtest/note.hpp"

namespace Soundtest
{
	class Melody
	{
	public:
		Melody(Sound::Sound sound, float bpm, DynArray<Note> notes);
		~Melody();

		void play();
		bool update(double _deltaTime);
		void setRepeat(bool repeat);
	private:
		void setPitch();

		Sound::Source *m_source;
		double m_curTime;
		float m_spb;
		DynArray<Note> m_notes;
		int m_index;
		bool m_repeat;
	};
}