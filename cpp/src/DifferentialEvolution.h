#ifndef DIFFERENTIAL_EVOLUTION_OPTIMIZATION_H
#define DIFFERENTIAL_EVOLUTION_OPTIMIZATION_H


#include "OptimizationAlg.h"
#include "Particle.h"


template< size_t __NUM_PARTICLES, size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class DifferentialEvolution : public OptimizationAlg< __NUM_PARTICLES, Particle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    static constexpr size_t NUM_PARTICLES = __NUM_PARTICLES;
    using Base = OptimizationAlg< __NUM_PARTICLES, Particle, __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using particle_t = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr param_t DEFAULT_MUTATION_FACTOR = 0.5;
    static constexpr param_t DEFAULT_CROSSOVER_PROBABILITY = 0.7;

    // Constructor
    DifferentialEvolution( const param_t lowerBound[__NUM_PARAMS], const param_t upperBound[__NUM_PARAMS], const int numThreads = 1 )
        : Base( lowerBound, upperBound, numThreads )
        , mutation_{ DEFAULT_MUTATION_FACTOR }
        , crossProb_{ DEFAULT_CROSSOVER_PROBABILITY }
    {
    }

    // Destructor
    virtual ~DifferentialEvolution() {}

    void setMutationFactor( const param_t mutation ) { mutation_ = mutation; }
    void setCrossoverProbability( const param_t crossProb ) { crossProb_ = crossProb; }


protected:

    void updateParticle( particle_t& particle ) override
    {
        // Mutation selection
        uint mutationIndices[3];
        this->rng_->choice( mutationIndices, NUM_PARTICLES - 1, false );

        const particle_t mutants[3] = {
            this->particles_[mutationIndices[0]],
            this->particles_[mutationIndices[1]],
            this->particles_[mutationIndices[2]]
        };


        // Mutation
        particle_t trialParticle = mutants[0] + mutation_ * ( mutants[1] - mutants[2] );


        // Crossover
        for ( size_t j = 0; j < particle.NUM_PARAMS; j++ )
        {
            if ( this->rng_->drawUniform( 0.0, 1.0 ) > crossProb_ )
            {
                trialParticle.position_[j] = particle.position_[j];
            }
        }

        trialParticle.clip( this->lowerBound_, this->upperBound_ );

        trialParticle.fitness_ = this->fitnessFunc_( trialParticle );

        // Selection
        if ( trialParticle.fitness_ < particle.fitness_ )
        {
            particle = trialParticle;
        }
    }


    param_t mutation_;
    param_t crossProb_;


    DifferentialEvolution() = delete;
    DifferentialEvolution( const DifferentialEvolution& ) = delete;
    DifferentialEvolution& operator=( const DifferentialEvolution& ) = delete;


}; // class DifferentialEvolution



#endif // DIFFERENTIAL_EVOLUTION_OPTIMIZATION_H