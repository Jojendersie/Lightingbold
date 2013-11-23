#include "random.hpp"
#include "../math/math.hpp"
#include <cmath>

namespace Generators {

        static uint32_t HashInt( uint32_t _a )
        {
                _a -= _a << 6;
                _a ^= _a >> 17;
                _a -= _a << 9;
                _a ^= _a << 4;
                _a -= _a << 3;
                _a ^= _a << 10;
                _a ^= _a >> 15;
                return _a;
        }


        // ********************************************************************* //
        Random::Random( uint32_t _seed )
        {
                m_state[0] = 0xaffeaffe ^ _seed;
                m_state[1] = 0xf9b2a750 ^ (_seed + 1);
                m_state[2] = 0x485eac66 ^ (_seed - 1);
                m_state[3] = 0xcbd02714 ^ (_seed + 263083);
        }

        // ********************************************************************* //
        float Random::Uniform( float _min, float _max )
        {
                double scale = (_max - _min) / 0xffffffff;
                return float(_min + scale * Xorshift128());
        }

        // ********************************************************************* //
        int32_t Random::Uniform( int32_t _min, int32_t _max )
        {
                uint32_t interval = uint32_t(_max - _min + 1);
                // Do not use interger maximum bounds!
                Assert(interval != 0);

                uint32_t value = Xorshift128();
                return _min + value % interval;
        }

        // ********************************************************************* //
        float Random::Normal( float _variance )
        {
                // Make several turns for a good quality (addition of two standard
                // normal distributions is again a standard normal distribution).
                double n = 0.0;
                for( int i=0; i<3; ++i )
                {
                        // Box-Muller method
                        float u1 = Uniform();
                        float u2 = Uniform();
                        double r;
                        if( u1 <= 0.0f ) r = 1487.0;        // -2*log(e-323) == 1487.493849482
                        else if( u1 >= 1.0f ) continue;
                        else r = sqrt(-2.0*log(u1));
                        // Use both indipendent samples to generate one sample of a
                        // heigher quality.
                        n += r*cos(6.28318530718*u2) + r*sin(6.28318530718*u2);
                }
                return float(sqrt(_variance / 6.0) * n);
        }

        // ********************************************************************* //
        float Random::Exponential( float _lambda )
        {
                double n = 0.0;
                for( int i=0; i<5; ++i )
                {
                        float u = Uniform();
                        if( u <= 0.0f ) n += 743.7;        // -log(e-323) == 743.746924741
                        else if( u >= 1.0f ) continue;
                        n += -log(u);
                }
                return float(n / (_lambda * 5.0));
        }

        uint32_t Random::Xorshift128()
        {
                uint32_t tmp = m_state[0] ^ (m_state[0] << 11);
                m_state[0] = m_state[1];
                m_state[1] = m_state[2];
                m_state[2] = m_state[3];
                m_state[3] ^= (m_state[3] >> 19) ^ tmp ^ (tmp >> 8);
 
                return m_state[3];
        }
};