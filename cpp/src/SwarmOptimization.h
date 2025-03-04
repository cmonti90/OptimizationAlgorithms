#ifndef SWARM_OPTIMIZATION_H
#define SWARM_OPTIMIZATION_H


#include "OptimizationAlg.h"
#include "SwarmParticle.h"



namespace MetaOpt
{

template< size_t __NUM_PARTICLES, size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmOptimization : public OptimizationAlg< __NUM_PARTICLES, SwarmParticle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base       = OptimizationAlg< __NUM_PARTICLES, SwarmParticle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using particle_t = SwarmParticle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t    = __PARAM_T;
    using fitness_t  = __FITNESS_T;


    static constexpr size_t  NUM_PARTICLES     = __NUM_PARTICLES;
    static constexpr param_t DEFAULT_INERTIA   = 0.5;
    static constexpr param_t DEFAULT_COGNITIVE = 1.0;
    static constexpr param_t DEFAULT_SOCIAL    = 1.0;



    // Constructor
    SwarmOptimization( const param_t lowerBound[__NUM_PARAMS], const param_t upperBound[__NUM_PARAMS], const int numThreads = 1 )
        : Base( lowerBound, upperBound, numThreads )
        , inertia_  { DEFAULT_INERTIA }
        , cognitive_{ DEFAULT_COGNITIVE }
        , social_   { DEFAULT_SOCIAL }
    {
    }

    // Destructor
    virtual ~SwarmOptimization() {}

    void setInertia  ( const param_t inertia )   { inertia_   = inertia; }
    void setCognitive( const param_t cognitive ) { cognitive_ = cognitive; }
    void setSocial   ( const param_t social )    { social_    = social; }


protected:


    virtual void updateParticle( particle_t& particle ) override
    {
        for ( size_t j = 0; j < particle.NUM_PARAMS; j++ )
        {
            particle.velocity_[j] = inertia_ * particle.velocity_[j] +
                                    cognitive_ * this->rng_->drawUniform( 0.0, 1.0 ) * ( particle.bestPosition_[j] - particle.position_[j] ) +
                                    social_ * this->rng_->drawUniform( 0.0, 1.0 ) * ( this->bestParticle_->position_[j] - particle.position_[j] );

            particle.position_[j] += particle.velocity_[j];
        }

        particle.clip( this->lowerBound_, this->upperBound_ );

        particle.fitness_ = this->fitnessFunc_( particle );
    }


    param_t inertia_;
    param_t cognitive_;
    param_t social_;


    SwarmOptimization() = delete;
    SwarmOptimization( const SwarmOptimization& ) = delete;
    SwarmOptimization& operator=( const SwarmOptimization& ) = delete;


}; // class SwarmOptimization


} // namespace MetaOpt

#endif // SWARM_OPTIMIZATION_H