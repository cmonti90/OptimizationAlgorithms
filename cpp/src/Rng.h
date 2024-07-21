#ifndef RNG_H
#define RNG_H

#include <random>


template< size_t __NUM_PARAMS, typename __PARAM_T, typename __FITNESS_T >
class Particle;


template< size_t __NUM_PARAMS, uint64_t __SEED = 0u, typename __PARAM_T = double, typename __GENERATOR = std::mt19937 >
class Rng
{
public:

    using param_t = __PARAM_T;
    using generator_t = __GENERATOR;


    static Rng* getInstance()
    {
        static Rng instance;
        return &instance;
    }

protected:

    Rng()
        : generator_{ __SEED }
        , realDistribution_{ 0.0, 1.0 }
        , intDistribution_{ 0, 1 }
    {
    }

    virtual ~Rng() {}

    template< typename fitness_t >
    void drawUniform( Particle< __NUM_PARAMS, __PARAM_T, fitness_t >& particle, const __PARAM_T lowerBound[__NUM_PARAMS], const __PARAM_T upperBound[__NUM_PARAMS] );
private:

    generator_t generator_;
    std::uniform_real_distribution< double > realDistribution_;
    std::uniform_int_distribution< int > intDistribution_;
}; // class Rng


#include "Particle.h"


template< size_t __NUM_PARAMS, uint64_t __SEED, typename __PARAM_T, typename __GENERATOR >
template< typename __FITNESS_T >
void Rng< __NUM_PARAMS, __SEED, __PARAM_T, __GENERATOR >::drawUniform( Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >& particle, const __PARAM_T lowerBound[__NUM_PARAMS], const __PARAM_T upperBound[__NUM_PARAMS] )
{
    for ( size_t i = 0; i < __NUM_PARAMS; ++i )
    {
        particle.position_[i] = lowerBound[i] + ( upperBound[i] - lowerBound[i] ) * static_cast< __PARAM_T >( realDistribution_( generator_ ) );
    }
}
#endif // RNG_H