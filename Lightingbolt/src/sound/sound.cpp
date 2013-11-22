#include "sound.hpp"
//#include "math.hpp"
//#include <al.h>

namespace Sound {

	// ********************************************************************* //
	// Create a sound from raw data
	Sound::Sound( const ALchar* _data, int _size ) :
		m_size( _size )
	{
		alGenBuffers( 1, &m_buffer );

		alBufferData( m_buffer, AL_FORMAT_MONO8, _data, _size, 20000 );
	}

	Sound::~Sound()
	{
		alDeleteBuffers( 1, &m_buffer );
	}

	// ********************************************************************* //
	Source::Source( const Sound& _sound, float _volume, float _referenceDistance )
	{
		alGenSources( 1, &m_uiSource );
		// Attach the sound to this source
		alSourcei( m_uiSource, AL_BUFFER, _sound.GetBufferID() );
		// The default setting is just ok.
		// TODO: max(0.01f, _referenceDistance)
		alSourcef( m_uiSource, AL_REFERENCE_DISTANCE, _referenceDistance );
		alSourcef( m_uiSource, AL_MAX_DISTANCE, 10000000.0f );
		alSourcef( m_uiSource, AL_GAIN, _volume );
	}

	Source::~Source()
	{
		// Clean exit
		alSourceStop( m_uiSource );
		alDeleteSources( 1, &m_uiSource );
	}

	void Source::Play() const
	{
		alSourcePlay( m_uiSource );
	}

	bool Source::IsPlaying() const
	{
		ALint iState;
		alGetSourcei( m_uiSource, AL_SOURCE_STATE, &iState );
		return iState == AL_PLAYING;
	}

	void Source::Stop() const
	{
		alSourceStop( m_uiSource );
	}

	void Source::Pause() const
	{
		alSourcePause( m_uiSource );
	}

	void Source::SetVelocity( const Math::Vec3& _v )
	{
		alSourcefv( m_uiSource, AL_VELOCITY, (float*)&_v );
	}

	void Source::SetPosition( const Math::Vec3& _p )
	{
		alSourcefv( m_uiSource, AL_POSITION, (float*)&_p );
	}
};