#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include "Particle.h"

#include "Semaphore.h"

#include "Timer.h"

#include <memory>

#include <cstddef>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>

template< size_t __NUM_PARTICLES, template< size_t, typename, typename > typename __PARTICLE_T, size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double,
          typename = typename std::enable_if<
              std::is_base_of<
                  Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >,
                  __PARTICLE_T< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
                  >::value
              >::type>
class OptimizationAlg
{
public:

    static_assert( __NUM_PARTICLES > 0, "Number of particles must be greater than 0" );


    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;

    static constexpr size_t NUM_PARTICLES = __NUM_PARTICLES;

    using particle_t = __PARTICLE_T< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    static constexpr size_t NUM_PARAMS = particle_t::NUM_PARAMS;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;


    OptimizationAlg( const param_t lowerBound[NUM_PARAMS], const param_t upperBound[NUM_PARAMS], const int numThreads = 1 )
        : upperBound_{}
        , lowerBound_{}
        , particles_{}
        , bestParticle_{ std::make_shared< BestParticle< particle_t > >() }
        , semaphore_{ numThreads }
        , threadingEnabled_{ false }
        , rng_{ Rng<>::getInstance() }
        , timer_{ Timer::getInstance() }
        , fitnessFunc_{ nullptr }
        , iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
    {
        std::memcpy( upperBound_, upperBound, NUM_PARAMS * sizeof( param_t ) );
        std::memcpy( lowerBound_, lowerBound, NUM_PARAMS * sizeof( param_t ) );
    }


    virtual ~OptimizationAlg() {}


    void run()
    {
        initializeParticles();

        postInitialize();

        evaluateParticles();
        updateBestParticle();

        std::cout << "Initial best particle position: [";

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            std::cout << bestParticle_->position_[i] << ( i < NUM_PARAMS - 1 ? ", " : "" );
        }

        std::cout << "]" << std::endl;

        std::cout << "Initial best particle fitness: " << bestParticle_->fitness_ << std::endl;

        std::cout << "/////////////////////////" << std::endl;

        for ( iteration_ = 0; iteration_ < maxIterations_; ++iteration_ )
        {
            iterate();

            if ( isConverged() )
            {
                break;
            }
        }

        std::cout << "Best particle position: [";

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            std::cout << bestParticle_->position_[i] << ( i < NUM_PARAMS - 1 ? ", " : "" );
        }

        std::cout << "]" << std::endl;

        std::cout << "Best particle fitness: " << bestParticle_->fitness_ << std::endl;

    }


    void setMaxIterations( const uint64_t maxIterations )
    {
        maxIterations_ = maxIterations;
    }


    void setFitnessFunc( std::function< fitness_t( particle_t& ) > fitnessFunc )
    {
        fitnessFunc_ = fitnessFunc;
    }


    void enableThreading() { threadingEnabled_ = true; }
    void disableThreading() { threadingEnabled_ = false; }
    bool isThreadingEnabled() const { return threadingEnabled_; }


    std::function< fitness_t( particle_t& ) > getFitnessFunc() const { return fitnessFunc_; }    


protected:

    virtual void postInitialize() {}


    virtual void updateParticle( particle_t& ) = 0;


    virtual void updateParticles()
    {
        timer_->startUpdateParticleLoop();

        if ( threadingEnabled_ )
        {
            std::mutex complMtx;
            std::condition_variable complCv;
            uint16_t numTasksCompleted = 0;

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                semaphore_.acquire();

                std::thread( [this, i, &complMtx, &complCv, &numTasksCompleted]()
                {
                    updateParticle( particles_[i] );

                    std::lock_guard< std::mutex > lock( complMtx );
                    ++numTasksCompleted;

                    semaphore_.release();
                    complCv.notify_one();

                } ).detach();
            }

            std::unique_lock< std::mutex > lock( complMtx );
            complCv.wait( lock, [&numTasksCompleted]() { return numTasksCompleted == NUM_PARTICLES; } );
        }
        else
        {
            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                updateParticle( particles_[i] );
            }
        }

        timer_->stopUpdateParticleLoop();
    }


    virtual void evaluateParticles()
    {
        timer_->startEvaluateParticleLoop();

        if ( threadingEnabled_ )
        {
            std::mutex complMtx;
            std::condition_variable complCv;
            uint16_t numTasksCompleted = 0;

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                semaphore_.acquire();

                std::thread( [this, i, &complMtx, &complCv, &numTasksCompleted]()
                {
                    particles_[i].fitness_ = fitnessFunc_( particles_[i] );

                    std::lock_guard< std::mutex > lock( complMtx );
                    ++numTasksCompleted;

                    semaphore_.release();
                    complCv.notify_one();

                } ).detach();
            }

            std::unique_lock< std::mutex > lock( complMtx );
            complCv.wait( lock, [&numTasksCompleted]() { return numTasksCompleted == NUM_PARTICLES; } );
        }
        else
        {
            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                particles_[i].fitness_ = fitnessFunc_( particles_[i] );
            }
        }

        timer_->stopEvaluateParticleLoop();
    }


    virtual bool isConverged() { return false; };


    param_t upperBound_[NUM_PARAMS];
    param_t lowerBound_[NUM_PARAMS];

    particle_t particles_[NUM_PARTICLES];

    std::shared_ptr< BestParticle< particle_t > > bestParticle_;

    Semaphore semaphore_;
    bool threadingEnabled_;

    Rng<>* rng_;
    Timer* timer_;

    std::function< fitness_t( particle_t& ) > fitnessFunc_;

private:

    void initializeParticles()
    {
        timer_->startInitializeParticleLoop();

        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            particles_[i].initialize( lowerBound_, upperBound_ );
        }

        timer_->stopInitializeParticleLoop();
    }


    virtual void iterate()
    {
        updateParticles();
        updateBestParticle();
    }


    void updateBestParticle()
    {
        this->bestParticle_->trialParticle( particles_ );
    }


    uint64_t iteration_;
    uint64_t maxIterations_;

    OptimizationAlg() = delete;


}; // class OptimizationAlg




#endif // OPTIMIZATIONALG_H