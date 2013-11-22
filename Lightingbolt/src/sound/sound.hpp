#pragma once

#include <al.h>
#include "../predecl.hpp"

namespace Sound {

	/**************************************************************************//**
	* \class 	Sound::Sound
	* \brief	The raw representation of the data of a sound.
	* \details	A sound stores the "wave" and is not playable by itself.
	*****************************************************************************/
	class Sound
	{
		ALuint m_uiBuffer;
		int	m_iSize;
	public:

		/// \brief Create a sound from raw data
		/// \param [in] _pcData A 20000 frequency sampled sound buffer.
		Sound( const ALchar* _pcData, int _iSize );

		~Sound();

		int GetBufferID() const		{ return m_uiBuffer; }
	};

	/**************************************************************************//**
	* \class 	Sound::Source
	* \brief	A source is a positioned sound which can be played...
	*****************************************************************************/
	class Source
	{
		ALuint m_uiSource;
	public:
		/// \param [in] _Sound The sound buffer which should be played at this
		///		source.
		/// \param [in] _fVolume The volume at the reference distance.
		/// \param [in] _fReferenceDistance Up to this distance the sound will
		///		have _fVolume constantly. Afterwards the sound fades out.
		Source( const Sound& _Sound, float _fVolume, float _fReferenceDistance );

		~Source();

		/// \brief Start playing the sound from the current position.
		///
		void Play() const;
		bool IsPlaying() const;

		/// \brief Stops playing the sound immediatelly.
		/// \details On play the sound starts from zero.
		void Stop() const;
		
		/// \brief Halts the sound such that it may be resumed by play.
		/// \details On play the sound starts from the position it was
		///		stopped before.
		void Pause() const;

		void SetVelocity( const Math::Vec3& _v );
		void SetPosition( const Math::Vec3& _p );
	};
};