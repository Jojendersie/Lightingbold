#include "soundmanager.hpp"

namespace Soundtest
{
	SoundManager::SoundManager()
	{
		Sound::Device::UseOpenAL();

		float frequency=261.63f;
		char *note=Soundtest::Signal::getSine(frequency);

		m_sound=new Sound::Sound(note, Soundtest::Signal::getLength(frequency));

		m_high=new Melody(*m_sound, 110, Song::twosonHigh());
		m_high->setRepeat(true);
		m_high->play();
		m_low=new Melody(*m_sound, 110, Song::twosonLow());
		m_low->setRepeat(true);
		m_low->play();
	}

	SoundManager::~SoundManager()
	{
		delete m_sound;
		delete m_high;
		delete m_low;
	}

	void SoundManager::update(double _deltaTime)
	{
		if(m_high->update(_deltaTime))
		{
			m_high->play();
			m_low->play();
		}
		m_low->update(_deltaTime);
	}
}