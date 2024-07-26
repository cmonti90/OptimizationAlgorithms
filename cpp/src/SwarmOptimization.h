#ifndef SWARM_OPTIMIZATION_H
#define SWARM_OPTIMIZATION_H


#include "OptimizationAlg.h"
#include "SwarmParticle.h"


template< size_t __NUM_PARTICLES, size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class SwarmOptimization : public OptimizationAlg< __NUM_PARTICLES, SwarmParticle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    static constexpr size_t NUM_PARTICLES = __NUM_PARTICLES;
    using Base = OptimizationAlg< __NUM_PARTICLES, SwarmParticle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using particle_t = SwarmParticle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr param_t DEFAULT_INERTIA = 0.5;
    static constexpr param_t DEFAULT_COGNITIVE = 1.0;
    static constexpr param_t DEFAULT_SOCIAL = 1.0;

    // Constructor
    SwarmOptimization( const param_t lowerBound[__NUM_PARAMS], const param_t upperBound[__NUM_PARAMS], const int numThreads = 1 )
        : Base( lowerBound, upperBound, numThreads )
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
    }


    virtual void updateParticles() override
    {
        if ( this->threadingEnabled_ )
        {
            std::mutex complMtx;
            std::condition_variable complCv;
            uint16_t numTasksCompleted = 0;

            for ( size_t i = 0; i < Base::NUM_PARTICLES; i++ )
            {
                this->semaphore_.acquire();

                std::thread( [this, i, &complMtx, &complCv, &numTasksCompleted]()
                {
                    updateParticle( this->particles_[i] );

                    std::lock_guard< std::mutex > lock( complMtx );
                    ++numTasksCompleted;

                    this->semaphore_.release();
                    complCv.notify_one();

                } ).detach();
            }

            std::unique_lock< std::mutex > lock( complMtx );
            complCv.wait( lock, [&numTasksCompleted]() { return numTasksCompleted == NUM_PARTICLES; } );
        }
        else
        {
            for ( size_t i = 0; i < Base::NUM_PARTICLES; i++ )
            {
                updateParticle( this->particles_[i] );
            }
        }

        this->evaluateParticles();
    }



    param_t inertia_;
    param_t cognitive_;
    param_t social_;


    SwarmOptimization() = delete;
    SwarmOptimization( const SwarmOptimization& ) = delete;
    SwarmOptimization& operator=( const SwarmOptimization& ) = delete;


}; // class SwarmOptimization



#endif // SWARM_OPTIMIZATION_H