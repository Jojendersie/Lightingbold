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
		ALuint m_buffer;
		int	m_size;
	public:

		/// \brief Create a sound from raw data
		/// \param [in] _data A 20000 frequency sampled sound buffer.
		Sound( const ALchar* _data, int _size );

		~Sound();

		int GetBufferID() const		{ return m_buffer; }
	};

	/**************************************************************************//**
	* \class 	Sound::Source
	* \brief	A source is a positioned sound which can be played...
	*****************************************************************************/
	class Source
	{
		ALuint m_uiSource;
	public:
		/// \param [in] _sound The sound buffer which should be played at this
		///		source.
		/// \param [in] _volume The volume at the reference distance.
		/// \param [in] _referenceDistance Up to this distance the sound will
		///		have _volume constantly. Afterwards the sound fades out.
		Source( const Sound& _sound, float _volume, float _referenceDistance );

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