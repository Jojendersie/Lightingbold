#include "sounddevice.hpp"
#include <alc.h>
#include <al.h>
#include <cassert>
#include "../math/math.hpp"

namespace Sound {

	Device::Device()
	{
		m_device = alcOpenDevice( nullptr );
		assert(m_device);
		if( m_device )
		{
			m_pContext = alcCreateContext( m_device, nullptr );
			assert(m_pContext);
			if( m_pContext )
			{
				alcMakeContextCurrent( m_pContext );
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
		alcDestroyContext( m_pContext );
		alcCloseDevice( m_device );
	}

	Device& Device::UseOpenAL()
	{
		static Device TheOneAndOnly;
		return TheOneAndOnly;
	}

	void Device::SetListenerVelocity( const Math::Vec3& _v )	{ alListenerfv( AL_VELOCITY, (float*)&_v ); }
	void Device::SetListenerPosition( const Math::Vec3& _p )	{ alListenerfv( AL_POSITION, (float*)&_p ); }
	void Device::SetListenerOrientation( const Math::Vec3& _vAt, const Math::Vec3& _vUp )
	{
		// Copy stuff to make sure it is in the correct order in memory
		float afOrientation[6] = { _vAt.x, _vAt.y, _vAt.z, _vUp.x, _vUp.y, _vUp.z };
		alListenerfv( AL_ORIENTATION, afOrientation );
	}
};