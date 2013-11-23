#pragma once

#include <cstdint>
#include "../predecl.hpp"

namespace Math {
        class Vec2;
        class Vec3;
};

namespace Generators {

        /// \brief A pseudo random number generator.
        /// \details The random number generator objects are idipendent of each
        ///                other. This allows deterministic sequences even in a multi-threaded
        ///                application.
        ///
        class Random {
        public:
                /// \brief Create a new independent random number generator with a seed.
                Random( uint32_t _seed );

                /// \brief Create the next uniform distributed sample in the given
                ///                range (inclusive both boundaries). Default is [0,1].
                float Uniform( float _min = 0.0f, float _max = 1.0f );

                /// \brief Create the next uniform distributed sample in the given
                ///                integer range (inclusive both boundaries).
                int32_t Uniform( int32_t _min, int32_t _max );

                /// \brief Create the next normal distributed sample with mu=0.
                float Normal( float _variance );

                /// \brief Create a random sample of an exponential distribution.
                float Exponential( float _lambda );

        private:
                /// \brief The real generator used in all sample functions.
                uint32_t Xorshift128();
                uint32_t m_state[4];        ///< Internal state of the generator
        };

        // Is there a need for real randomness?
	    float RealUniformRandom( float _min = 0.0f, float _max = 1.0f );
        int32_t RealUniformRandom( int32_t _min, int32_t _max );

        float RealNormalRandom( float _variance );
};