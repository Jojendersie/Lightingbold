#include "sounddevice.hpp"
#include <alc.h>
#include <al.h>
#include "../math/math.hpp"

namespace Sound {

	Device::Device()
	{
		m_device = alcOpenDevice( nullptr );
		Assert(m_device);
		if( m_device )
		{
			m_context = alcCreateContext( m_device, nullptr );
			Assert(m_context);
			if( m_context )
			{
				alcMakeContextCurrent( m_context );
			} else {
				alcCloseDevice( m_device );
			}
		}

		// Set global default properties
		alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED );
	}

	Device::~Device()
	{
		alcMakeContextCurrent( nullptr );
		alcDestroyContext( m_context );
		alcCloseDevice( m_device );
	}

	Device& Device::UseOpenAL()
	{
		static Device TheOneAndOnly;
		return TheOneAndOnly;
	}

	void Device::SetListenerVelocity( const Math::Vec3& _v )	{ alListenerfv( AL_VELOCITY, (float*)&_v ); }
	void Device::SetListenerPosition( const Math::Vec3& _p )	{ alListenerfv( AL_POSITION, (float*)&_p ); }
	void Device::SetListenerOrientation( const Math::Vec3& _at, const Math::Vec3& _up )
	{
		// Copy stuff to make sure it is in the correct order in memory
		float orientation[6] = { _at.x, _at.y, _at.z, _up.x, _up.y, _up.z };
		alListenerfv( AL_ORIENTATION, orientation );
	}
};