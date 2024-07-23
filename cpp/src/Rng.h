#ifndef RNG_H
#define RNG_H

#include <random>

#include <mutex>


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


    template< typename float_t = double >
    float_t drawUniform( const float_t lowerBound, const float_t upperBound );

private:


    Rng()
        : generator_{ 0 }
        , realDistribution_{ 0.0, 1.0 }
        , intDistribution_{ 0, 1 }
        , mutex_{}
    {
    }

    generator_t generator_;
    std::uniform_real_distribution< double > realDistribution_;
    std::uniform_int_distribution< int > intDistribution_;

    std::mutex mutex_;

    Rng( const Rng& ) = delete;
    Rng& operator=( const Rng& ) = delete;
}; // class Rng


template< typename __GENERATOR >
template< typename float_t >
float_t Rng< __GENERATOR >::drawUniform( const float_t lowerBound, const float_t upperBound )
{
    std::lock_guard< std::mutex > lock( mutex_ );

    return static_cast< float_t >( lowerBound + ( upperBound - lowerBound ) * realDistribution_( generator_ ) );
}
#endif // RNG_H