#ifndef RNG_H
#define RNG_H

#include <algorithm>
#include <cstring>
#include <random>
#include <mutex>
#include <numeric>


template< typename __GENERATOR = std::mt19937 >
class Rng
{
public:

    using generator_t = __GENERATOR;


    virtual ~Rng() {}


    static Rng* getInstance()
    {
        static Rng instance;
        return &instance;
    }


    void setSeed( const uint64_t seed )
    {
        generator_.seed( seed );
    }


    template< typename float_t >
    inline float_t drawUniform( const float_t lowerBound = static_cast< float_t >( 0 ), const float_t upperBound = static_cast< float_t >( 1 ) )
    {
        std::uniform_real_distribution< float_t > realDistribution_{ lowerBound, upperBound };

        std::lock_guard< std::mutex > lock( mutex_ );

        return realDistribution_( generator_ );
    }


    template< typename float_t, size_t N >
    inline void drawUniform( float_t ( &out )[N], const float_t lowerBound = static_cast< float_t >( 0 ), const float_t upperBound = static_cast< float_t >( 1 ) )
    {
        std::uniform_real_distribution< float_t > realDistribution_{ lowerBound, upperBound };

        std::lock_guard< std::mutex > lock( mutex_ );

        for ( size_t i = 0; i < N; ++i )
        {
            out[i] = realDistribution_( generator_ );
        }
    }


    template< typename int_t >
    inline int_t drawUniformInt( const int_t lowerBound, const int_t upperBound )
    {
        std::uniform_int_distribution< int_t > intDistribution_{ lowerBound, upperBound };

        std::lock_guard< std::mutex > lock( mutex_ );

        return intDistribution_( generator_ );
    }


    template< size_t N >
    void choice( uint ( &out)[N], const uint upperBound, const bool replace = false )
    {
        std::uniform_int_distribution< uint > intDistribution_{ 0, upperBound - 1 };

        std::lock_guard< std::mutex > lock( mutex_ );

        if ( replace )
        {
            for ( size_t i = 0; i < N; i++ )
            {
                out[i] = intDistribution_( generator_ );
            }
        }
        else
        {
            if ( N > upperBound )
            {
                throw std::invalid_argument( "Rng::choice: N must be less than or equal to upperBound" );
            }

            uint choices[upperBound];
            std::iota( choices, choices + upperBound, 0u );
            
            std::shuffle( choices, choices + upperBound, generator_ );

            std::memcpy( out, choices, N * sizeof( uint ) );
        }
    }


private:


    Rng()
        : generator_{ 0 }
        , mutex_{}
    {
    }

    generator_t generator_;

    std::mutex mutex_;

    Rng( const Rng& ) = delete;
    Rng& operator=( const Rng& ) = delete;
}; // class Rng


#endif // RNG_H