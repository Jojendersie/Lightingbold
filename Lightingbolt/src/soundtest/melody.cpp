#include "melody.hpp"

namespace Soundtest
{
	Melody::Melody(Sound::Sound sound, float bpm, vector<int> notes):
		m_curTime(0), m_spb(60.0f/bpm), m_notes(notes), m_index(0)
	{
		m_source=new Sound::Source(sound, 0.025f, 5);
		setPitch();
		m_source->SetLooping(true);
		m_source->Play();
	}

	Melody::~Melody()
	{
		delete m_source;
	}

	void Melody::update(double _deltaTime)
	{
		setPitch();

		m_curTime+=_deltaTime;
		if(m_curTime>=m_spb)
		{
			m_index++;
			
			m_curTime=0;
		}
		/*if(m_curTime<=m_notes.size())
		{
			int note=m_notes[(int)m_curTime];
			float pitch=pow(2.0, note*1.0/12.0);
			m_source->SetPitch(pitch);
		}*/
	}

	void Melody::setPitch()
	{
		if(m_index<m_notes.size())
		{
			int note=m_notes[m_index];
			float pitch=pow(2.0, note*1.0/12.0);
			m_source->SetPitch(pitch);
		}
	}
}