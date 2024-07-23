#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include "Particle.h"

#include <cstddef>
#include <functional>

#include <iostream>

template<size_t __NUM_PARTICLES, typename __PARTICLE_T,
         typename = typename std::enable_if<
             std::is_base_of<
                 Particle<__PARTICLE_T::NUM_PARAMS, typename __PARTICLE_T::param_t, typename __PARTICLE_T::fitness_t>,
                 __PARTICLE_T
                 >::value
             >::type>
class OptimizationAlg
{
public:

    static_assert( __NUM_PARTICLES > 0, "Number of particles must be greater than 0" );


    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;

    static constexpr size_t NUM_PARTICLES = __NUM_PARTICLES;

    using particle_t = __PARTICLE_T;
    static constexpr size_t NUM_PARAMS = particle_t::NUM_PARAMS;
    using param_t = typename particle_t::param_t;
    using fitness_t = typename particle_t::fitness_t;


    OptimizationAlg( const param_t lowerBound[NUM_PARAMS], const param_t upperBound[NUM_PARAMS] )
        : upperBound_{}
        , lowerBound_{}
        , particles_{}
        , bestParticle_{ std::make_shared< BestParticle< particle_t > >() }
        , iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
        , fitnessFunc_{ nullptr }
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


    virtual void updateParticles() = 0;


protected:

    virtual void postInitialize() {}

    param_t upperBound_[NUM_PARAMS];
    param_t lowerBound_[NUM_PARAMS];

    particle_t particles_[NUM_PARTICLES];

    std::shared_ptr< BestParticle< particle_t > > bestParticle_;

private:

    void initializeParticles()
    {
        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            particles_[i].initialize( lowerBound_, upperBound_ );
        }
    }


    void evaluateParticles()
    {
        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            particles_[i].fitness_ = fitnessFunc_( particles_[i] );
        }
    }


    virtual void iterate()
    {
        updateParticles();
        evaluateParticles();
        updateBestParticle();
    }


    virtual bool isConverged() { return false; };


    void updateBestParticle()
    {
        this->bestParticle_->trialParticle( particles_ );
    }


    uint64_t iteration_;
    uint64_t maxIterations_;

    std::function< fitness_t( particle_t& ) > fitnessFunc_;


}; // class OptimizationAlg




#endif // OPTIMIZATIONALG_H