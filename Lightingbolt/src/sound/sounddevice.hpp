#pragma once

#include "../predecl.hpp"

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace Sound {
	/**************************************************************************//**
	* \class 	Sound::Device
	* \brief	To play sounds one device is necessary.
	* \details	Singleton class for the sound device.
	*
	*			There is also just one listener which can be controlled for this
	*			device.
	*****************************************************************************/
	class Device
	{
		Device();
		~Device();

		// Hide copy and assignement
		Device( const Device& );
		void operator=(const Device&);

		ALCdevice* m_pDevice;
		ALCcontext* m_pContext;
	public:

		/// \brief Singleton access function.
		/// \details Only accessing the singleton is enough to have a device
		///		and a context. You will not need to touch this class later.
		static Device& UseOpenAL();

		/// \brief Sets the listeners velocity for doppler effect.
		///
		static void SetListenerVelocity( const Math::Vec3& _v );

		/// \brief Sets the listners position for 3D sound.
		///
		static void SetListenerPosition( const Math::Vec3& _p );

		/// \brief Sets an orthogonal base for the orientation of the listener.
		///
		static void SetListenerOrientation( const Math::Vec3& _vAt, const Math::Vec3& _vUp );
	};
};