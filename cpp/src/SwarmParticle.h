#ifndef SWARMPARTICLE_H
#define SWARMPARTICLE_H

#include "Particle.h"


template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmParticle : public Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr param_t DEFAULT_INERTIA = 0.5;
    static constexpr param_t DEFAULT_COGNITIVE = 1.0;
    static constexpr param_t DEFAULT_SOCIAL = 1.0;

    // Constructor
    SwarmParticle()
        : Base()
        , globalBestFitness_{ nullptr }
        , inertia_{ DEFAULT_INERTIA }
        , cognitive_{ DEFAULT_COGNITIVE }
        , social_{ DEFAULT_SOCIAL }
    {
        std::memset( velocity_, 0, Base::NUM_PARAMS * sizeof( param_t ) );
        std::memset( bestPosition_, 0, Base::NUM_PARAMS * sizeof( param_t ) );
    }

    // Destructor
    virtual ~SwarmParticle() {}

    // Copy constructor
    SwarmParticle( const SwarmParticle& other )
        : Base( other )
        , globalBestFitness_{ other.globalBestFitness_ }
        , inertia_{ other.inertia_ }
        , cognitive_{ other.cognitive_ }
        , social_{ other.social_ }
    {
        std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
        std::memcpy( bestPosition_, other.bestPosition_, Base::NUM_PARAMS * sizeof( param_t ) );
    }

    // Assignment operator
    SwarmParticle& operator=( const SwarmParticle& other )
    {
        if ( this != &other )
        {
            Base::operator=( other );

            std::memcpy( velocity_, other.velocity_, Base::NUM_PARAMS * sizeof( param_t ) );
            std::memcpy( bestPosition_, other.bestPosition_, Base::NUM_PARAMS * sizeof( param_t ) );

            globalBestFitness_ = other.globalBestFitness_;

            inertia_   = other.inertia_;
            cognitive_ = other.cognitive_;
            social_    = other.social_;
        }

        return *this;
    }

    void setInertia( const param_t inertia ) { inertia_ = inertia; }
    void setCognitive( const param_t cognitive ) { cognitive_ = cognitive; }
    void setSocial( const param_t social ) { social_ = social; }

protected:

    param_t velocity_[Base::NUM_PARAMS];

    param_t bestPosition_[Base::NUM_PARAMS];

    param_t* globalBestPosition_;
    fitness_t* globalBestFitness_;

    param_t inertia_;
    param_t cognitive_;
    param_t social_;

}; // class SwarmParticle

#endif // SWARMPARTICLE_H