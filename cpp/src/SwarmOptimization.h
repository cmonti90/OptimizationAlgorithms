#ifndef SWARM_OPTIMIZATION_H
#define SWARM_OPTIMIZATION_H

#include "OptimizationAlg.h"
#include "SwarmParticle.h"

template< size_t __NUM_PARTICLES, size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmOptimization : public OptimizationAlg< __NUM_PARTICLES, SwarmParticle< __NUM_PARAMS, __PARAM_T, __FITNESS_T > >
{
public:

    using Base = OptimizationAlg< __NUM_PARTICLES, SwarmParticle< __NUM_PARAMS, __PARAM_T, __FITNESS_T > >;
    using particle_t = SwarmParticle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = typename particle_t::param_t;
    using fitness_t = typename particle_t::fitness_t;

    static constexpr param_t DEFAULT_INERTIA = 0.5;
    static constexpr param_t DEFAULT_COGNITIVE = 1.0;
    static constexpr param_t DEFAULT_SOCIAL = 1.0;

    // Constructor
    SwarmOptimization()
        : Base()
        , inertia_{ DEFAULT_INERTIA }
        , cognitive_{ DEFAULT_COGNITIVE }
        , social_{ DEFAULT_SOCIAL }
    {
    }

    // Destructor
    virtual ~SwarmOptimization() {}

    void setInertia( const param_t inertia ) { inertia_ = inertia; }
    void setCognitive( const param_t cognitive ) { cognitive_ = cognitive; }
    void setSocial( const param_t social ) { social_ = social; }
    

private:

    virtual void updateParticles() override;

    param_t inertia_;
    param_t cognitive_;
    param_t social_;


}; // class SwarmOptimization


template< size_t __NUM_PARTICLES, size_t __NUM_PARAMS, typename __PARAM_T, typename __FITNESS_T >
void SwarmOptimization< __NUM_PARTICLES, __NUM_PARAMS, __PARAM_T, __FITNESS_T >::updateParticles()
{
    for ( size_t i = 0; i < Base::NUM_PARTICLES; i++ )
    {
        particle_t& particle = this->particles_[i];

        for ( size_t j = 0; j < particle.NUM_PARAMS; j++ )
        {
            particle.velocity_[j] = inertia_ * particle.velocity_[j] +
                                    cognitive_ /** Rng::getUniform() */* ( particle.bestPosition_[j] - particle.position_[j] ) +
                                    social_ /** Rng::getUniform() */* ( this->bestParticle_->position_[j] - particle.position_[j] );

            particle.position_[j] += particle.velocity_[j];
        }
    }
}


#endif // SWARM_OPTIMIZATION_H