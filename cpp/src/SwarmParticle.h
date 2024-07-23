#ifndef SWARMPARTICLE_H
#define SWARMPARTICLE_H

#include "Particle.h"

#include <memory>

template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmParticle : public Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    // Constructor
    SwarmParticle()
        : Base()
        , velocity_{}
        , bestParticle_{ nullptr }
    {
        std::memset( velocity_, static_cast< param_t >( 0 ), Base::NUM_PARAMS * sizeof( param_t ) );
    }

    // Destructor
    virtual ~SwarmParticle() {}

    // Copy constructor
    SwarmParticle( const SwarmParticle& other )
        : Base( other )
        , velocity_{ other.velocity_ }
        , bestParticle_{ other.bestParticle_ }
    {
        std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
    }

    // Assignment operator
    SwarmParticle& operator=( const SwarmParticle& other )
    {
        if ( this != &other )
        {
            Base::operator=( other );

            std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
            std::memcpy( bestPosition_, other.bestPosition_, Base::NUM_PARAMS * sizeof( param_t ) );

            bestParticle_ = other.bestParticle_;
        }

        return *this;
    }

    // operator Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >() const
    // {
    //     Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T > p;

    //     std::memcpy( p.position_, Base::position_, Base::NUM_PARAMS * sizeof( param_t ) );
    //     p.fitness_ = Base::fitness_;

    //     return p;
    // }

    // operator Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >& ()
    // {
    //     return *this;
    // }


    void setBestParticle( const std::shared_ptr< SwarmParticle >& bestParticle )
    {
        bestParticle_ = bestParticle;
    }


    param_t velocity_[Base::NUM_PARAMS];

    param_t bestPosition_[Base::NUM_PARAMS];

protected:

    std::shared_ptr< SwarmParticle > bestParticle_;


}; // class SwarmParticle

#endif // SWARMPARTICLE_H