#pragma once

#include "../sound/sounddevice.hpp"
#include "melody.hpp"
#include "note.hpp"
#include "signal.hpp"
#include "song.hpp"

namespace Soundtest
{
	class SoundManager
	{
	public:
		SoundManager();
		~SoundManager();

		void update(double _deltaTime);
	private:
		Sound::Sound *m_sound;
		Melody *m_high;
		Melody *m_low;
	};
}