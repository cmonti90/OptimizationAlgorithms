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
        , inertia_{ DEFAULT_INERTIA }
        , cognitive_{ DEFAULT_COGNITIVE }
        , social_{ DEFAULT_SOCIAL }
    {
    }

    // Destructor
    virtual ~DifferentialEvolution() {}

    void setInertia( const param_t inertia ) { inertia_ = inertia; }
    void setCognitive( const param_t cognitive ) { cognitive_ = cognitive; }
    void setSocial( const param_t social ) { social_ = social; }


private:

    virtual void postInitialize() override
    {
        for ( size_t i = 0; i < Base::NUM_PARTICLES; i++ )
        {
            this->particles_[i].setBestParticle( this->bestParticle_ );
        }
    }

    virtual void updateParticles() override
    {
        static Rng<>* rng = Rng<>::getInstance();
        

        for ( size_t i = 0; i < Base::NUM_PARTICLES; i++ )
        {
            particle_t& particle = this->particles_[i];
            
            // Mutation selection
            uint mutationIndices[3];
            rng->choice( mutationIndices, Base::NUM_PARTICLES - 1, false );

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
                if ( rng->drawUniform< param_t >() > crossProb_ )
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
    }



    param_t mutation_;
    param_t crossProb_;


    DifferentialEvolution() = delete;
    DifferentialEvolution( const DifferentialEvolution& ) = delete;
    DifferentialEvolution& operator=( const DifferentialEvolution& ) = delete;


}; // class DifferentialEvolution



#endif // DIFFERENTIAL_EVOLUTION_OPTIMIZATION_H