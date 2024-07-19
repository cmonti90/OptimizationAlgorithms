#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include <cstddef>

template< size_t __NUM_PARICLES__, typename __PARICLE_T__ >
class OptimizationAlg
{
public:
    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;

    static constexpr size_t NUM_PARTICLES = __NUM_PARICLES__;
    typedef __PARICLE_T__ particle_t;


    OptimizationAlg()
        : iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
        , seed_{ 0 }
    {
    }


    virtual ~OptimizationAlg() {}


    void run()
    {
        initializeParticles();

        for( size_t i = 0; i < maxIterations_; i++ )
        {
            iterate();

            if( isConverged() )
            {
                break;
            }
        }
    
    }


    void setMaxIterations( const size_t maxIterations ) 
    {
        maxIterations_ = maxIterations;
    }


    void setSeed( const uint64_t seed )
    {
        seed_ = seed;
    }


private:

    void initializeParticles() 
    {
        for( size_t i = 0; i < NUM_PARTICLES; i++ )
        {
            particles_[i].initialize();
        }
    
    }

    virtual void iterate() = 0;

    virtual bool isConverged() { return false; };


    particle_t particles_[NUM_PARTICLES];

    size_t iteration_;
    size_t maxIterations_;

    uint64_t seed_;


}; // class OptimizationAlg




#endif // OPTIMIZATIONALG_H