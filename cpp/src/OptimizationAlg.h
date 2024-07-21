#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include <cstddef>
#include <functional>

template< size_t __NUM_PARICLES, typename __PARICLE_T >
class OptimizationAlg
{
    static_assert( __NUM_PARICLES > 0, "Number of particles must be greater than 0" );
    static_assert( std::is_base_of< Particle< __PARICLE_T::NUM_PARAMS, typename __PARICLE_T::param_t, typename __PARICLE_T::fitness_t >, __PARICLE_T >::value, "Particle type must be derived from Particle" );

public:
    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;

    static constexpr size_t NUM_PARTICLES = __NUM_PARICLES;
    using particle_t = __PARICLE_T;
    using param_t = typename particle_t::param_t;
    using fitness_t = typename particle_t::fitness_t;


    OptimizationAlg()
        : particles_{}
        , bestParticle_{ std::make_shared< BestParticle< NUM_PARTICLES, param_t, fitness_t > >() }
        , iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
        , seed_{ 0 }
        , fitnessFunc_{ nullptr }
    {
    }


    virtual ~OptimizationAlg() {}


    void run()
    {
        initializeParticles();

        for ( iteration_ = 0; iteration_ < maxIterations_; ++iteration_ )
        {
            iterate();

            if ( isConverged() )
            {
                break;
            }
        }
    }


    void setMaxIterations( const uint64_t maxIterations )
    {
        maxIterations_ = maxIterations;
    }


    void setSeed( const uint64_t seed )
    {
        seed_ = seed;
    }


    void setFitnessFunc( std::function< fitness_t( particle_t& ) > fitnessFunc )
    {
        fitnessFunc_ = fitnessFunc;
    }

    virtual void updateParticles() = 0;

protected:

    particle_t particles_[NUM_PARTICLES];
    
    param_t* upperBound_;
    param_t* lowerBound_;

    std::shared_ptr< BestParticle< NUM_PARTICLES, param_t, fitness_t > > bestParticle_;

private:

    void initializeParticles()
    {
        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            particles_[i].initialize();
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

    uint64_t seed_;

    std::function< fitness_t( particle_t& ) > fitnessFunc_;


}; // class OptimizationAlg




#endif // OPTIMIZATIONALG_H