#ifndef SWARMPARTICLE_H
#define SWARMPARTICLE_H

#include "Particle.h"

#include <memory>


namespace MetaOpt
{

template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmParticle : public Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base      = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t   = __PARAM_T;
    using fitness_t = __FITNESS_T;


    // Constructor
    inline SwarmParticle()
        : Base()
        , velocity_{}
    {
        std::memset( velocity_, 0, Base::NUM_PARAMS * sizeof( param_t ) );
    }


    // Destructor
    inline virtual ~SwarmParticle() {}


    // Copy constructor
    inline SwarmParticle( const param_t ( &other )[Base::NUM_PARAMS] )
        : Base( other )
        , velocity_{}
    {
        std::memset( velocity_, 0, Base::NUM_PARAMS * sizeof( param_t ) );
    }


    // Copy constructor
    inline SwarmParticle( const SwarmParticle& other )
        : Base( other )
        , velocity_{ other.velocity_ }
    {
        std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
    }


    // Copy constructor
    inline SwarmParticle( const std::initializer_list< param_t >& other )
        : Base( other )
        , velocity_{}
    {
        if ( other.size() != Base::NUM_PARAMS )
        {
            throw std::invalid_argument( "Initializer list size does not match number of parameters" );
        }

        std::memset( velocity_, 0, Base::NUM_PARAMS * sizeof( param_t ) );
    }


    // Assignment operator
    inline SwarmParticle& operator=( const SwarmParticle& other )
    {
        if ( this != &other )
        {
            Base::operator=( other );

            std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
            std::memcpy( bestPosition_, other.bestPosition_, Base::NUM_PARAMS * sizeof( param_t ) );
        }

        return *this;
    }


    param_t velocity_[Base::NUM_PARAMS];

    param_t bestPosition_[Base::NUM_PARAMS];


}; // class SwarmParticle

} // namespace MetaOpt

#endif // SWARMPARTICLE_H