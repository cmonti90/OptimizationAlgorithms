#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include "Particle.h"

#include "Semaphore.h"

#include "Timer.h"
#include "ThreadPool.h"
#include "Rng.h"


#include <array>
#include <cstddef>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <iostream>


namespace MetaOpt
{

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


    using particle_t = __PARTICLE_T< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t    = __PARAM_T;
    using fitness_t  = __FITNESS_T;


    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;
    static constexpr size_t NUM_PARTICLES          = __NUM_PARTICLES;
    static constexpr size_t NUM_PARAMS             = particle_t::NUM_PARAMS;



    OptimizationAlg( const param_t lowerBound[NUM_PARAMS], const param_t upperBound[NUM_PARAMS], const int numThreads = 1 )
        : upperBound_{}
        , lowerBound_{}
        , particles_{}
        , bestParticle_{ std::make_shared< BestParticle< particle_t > >() }
        , threadingEnabled_{ numThreads > 1 }
        , threadPool_{ numThreads }
        , rng_{ Rng<>::getInstance() }
        , timer_{ Timer::getInstance() }
        , fitnessFunc_{ nullptr }
        , iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
        , particleInsertIdx_{ 0u }
    {
        std::memcpy( upperBound_, upperBound, NUM_PARAMS * sizeof( param_t ) );
        std::memcpy( lowerBound_, lowerBound, NUM_PARAMS * sizeof( param_t ) );
    }


    virtual ~OptimizationAlg() {}


    void insertParticle( const particle_t& particle )
    {
        particles_[particleInsertIdx_++] = particle;
    }


    void run()
    {
        if ( threadingEnabled_ )
        {
            threadPool_.startThreads();

        }

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


        for ( iteration_ = 0; iteration_ < maxIterations_ & !isConverged(); ++iteration_ )
        {
            std::cout << "iteration: " << iteration_ << std::endl;

            iterate();
        }


        if ( threadingEnabled_ )
        {
            threadPool_.stopThreads();
        }


        std::cout << "Best particle position: [";

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            std::cout << bestParticle_->position_[i] << ( i < NUM_PARAMS - 1 ? ", " : "" );
        }

        std::cout << "]" << std::endl;

        std::cout << "Best particle fitness: " << bestParticle_->fitness_ << std::endl;

        particleInsertIdx_ = 0u;
    }


    void setMaxIterations( const uint64_t maxIterations )
    {
        maxIterations_ = maxIterations;
    }


    void setFitnessFunc( std::function< fitness_t( const particle_t& ) > fitnessFunc )
    {
        fitnessFunc_ = fitnessFunc;
    }


    bool isThreadingEnabled() const { return threadingEnabled_; }


    std::function< fitness_t( const particle_t& ) > getFitnessFunc() const { return fitnessFunc_; }

    particle_t getBestParticle() const { return *bestParticle_; }


protected:

    virtual void postInitialize() {}


    virtual void updateParticle( particle_t& ) = 0;


    virtual void updateParticles()
    {
        timer_->startUpdateParticleLoop();

        if ( threadingEnabled_ )
        {
            std::array< std::future< void >, NUM_PARTICLES > futures;

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                futures[i] = threadPool_.enqueue( [this, i]()
                {
                    updateParticle( particles_[i] );
                } );
            }

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                futures[i].get();
            }


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
            std::array< std::future< void >, NUM_PARTICLES > futures;

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                futures[i] = threadPool_.enqueue( [this, i]()
                {
                    particles_[i].fitness_ = fitnessFunc_( particles_[i] );
                } );
            }

            for ( size_t i = 0; i < NUM_PARTICLES; ++i )
            {
                futures[i].get();
            }
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

    bool threadingEnabled_;
    ThreadPool threadPool_;

    Rng<>* rng_;
    Timer* timer_;

    std::function< fitness_t( const particle_t& ) > fitnessFunc_;

private:

    void initializeParticles()
    {
        timer_->startInitializeParticleLoop();

        for ( size_t i = particleInsertIdx_; i < NUM_PARTICLES; ++i )
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

    uint32_t particleInsertIdx_;


    OptimizationAlg() = delete;
    OptimizationAlg( const OptimizationAlg& ) = delete;
    OptimizationAlg& operator=( const OptimizationAlg& ) = delete;

}; // class OptimizationAlg


} // namespace MetaOpt


#endif // OPTIMIZATIONALG_H