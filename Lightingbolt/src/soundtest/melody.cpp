#include "melody.hpp"

namespace Soundtest
{
	Melody::Melody(Sound::Sound sound, float bpm, DynArray<Note> notes):
		m_curTime(0), m_spb(60.0f/bpm), m_notes(notes), m_index(0), m_repeat(false)
	{
		m_source=new Sound::Source(sound, 0.03f, 5);
		m_source->SetLooping(true);
		if(m_index<m_notes.size())
		{
			setPitch();
			m_source->Stop();
			if(m_notes[m_index]->m_played)
			{
				m_source->Play();
			}
		}
	}

	Melody::~Melody()
	{
		delete m_source;
	}

	void Melody::update(double _deltaTime)
	{
		m_curTime+=_deltaTime;
		if(m_index<m_notes.size())
		{
			setPitch();
			if(m_curTime>=m_spb*m_notes[m_index]->m_value)
			{
				if(m_index<m_notes.size()-1)
				{
					m_index++;
				}
				else if(m_repeat)
				{
					m_index=0;
				}

				if(m_notes[m_index]->m_played)
				{
					m_source->Play();
				}
				else
				{
					m_source->Stop();
				}

				m_curTime=0;
			}
		}
		/*else
		{
			if(m_repeat)
			{
				m_index=0;
			}
		}*/
	}

	void Melody::setRepeat(bool repeat)
	{
		m_repeat=repeat;
	}

	void Melody::setPitch()
	{
		float pitch=(float)pow(2.0, m_notes[m_index]->m_note/12.0);
		m_source->SetPitch(pitch);
	}
}