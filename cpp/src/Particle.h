#ifndef PARTICLE_H
#define PARTICLE_H

#include "Rng.h"

#include <cstddef>
#include <cstring>
#include <limits>

#include <iostream>


template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class Particle
{
public:

    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr size_t NUM_PARAMS = __NUM_PARAMS;

    // Constructor
    inline Particle()
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memset( position_, static_cast< param_t >( 0 ), NUM_PARAMS * sizeof( param_t ) );
    }


    // Destructor
    inline virtual ~Particle() {}


    // Copy Constructor
    inline Particle( const Particle& other )
        : position_{}
        , fitness_{ other.fitness_ }
    {
        std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
    }


    // Assignment operator
    inline Particle& operator=( const Particle& other )
    {
        if ( this != &other )
        {
            std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
            fitness_ = other.fitness_;
        }

        return *this;
    }


    inline void initialize( const param_t lowerBounds[NUM_PARAMS], const param_t upperBounds[NUM_PARAMS] )
    {
        Rng<>* rng = Rng<>::getInstance();

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] = rng->drawUniform( lowerBounds[i], upperBounds[i] );
        }
    }


    inline void clip( const param_t lowerBounds[NUM_PARAMS], const param_t upperBounds[NUM_PARAMS] )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            if ( position_[i] < lowerBounds[i] )
            {
                position_[i] = lowerBounds[i];
            }
            else if ( position_[i] > upperBounds[i] )
            {
                position_[i] = upperBounds[i];
            }
        }
    }


    inline fitness_t getFitness() const { return fitness_; };

    inline bool operator==( const Particle& other ) const { return fitness_ == other.fitness_; }
    inline bool operator!=( const Particle& other ) const { return fitness_ != other.fitness_; }
    inline bool operator>( const Particle& other ) const { return fitness_ > other.fitness_; }
    inline bool operator<( const Particle& other ) const { return fitness_ < other.fitness_; }
    inline bool operator>=( const Particle& other ) const { return fitness_ >= other.fitness_; }
    inline bool operator<=( const Particle& other ) const { return fitness_ <= other.fitness_; }

    param_t position_[NUM_PARAMS];
    fitness_t fitness_;


}; // class Particle



template< typename __PARTICLE_T >
class BestParticle : public __PARTICLE_T
{
public:

    using Base = __PARTICLE_T;


    inline BestParticle()
        : Base{}
    {}


    inline virtual ~BestParticle() {}


    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;
    using Base::operator>;
    using Base::operator<;
    using Base::operator>=;
    using Base::operator<=;


    inline bool trialParticle( const Base& particle )
    {
        if ( particle < *this )
        {
            *this = particle;
            return true;
        }

        return false;
    }


    template< size_t NUM_PARTICLES >
    inline bool trialParticle( const Base ( &particles )[NUM_PARTICLES] )
    {
        bool updated = false;

        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            if ( trialParticle( particles[i] ) )
            {
                updated = true;
            }
        }

        return updated;
    }


}; // class BestParticle

#endif // PARTICLE_H