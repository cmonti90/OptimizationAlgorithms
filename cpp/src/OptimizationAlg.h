#ifndef OPTIMIZATIONALG_H
#define OPTIMIZATIONALG_H

#include <cstddef>

template< size_t __NUM_PARICLES, typename __PARICLE_T >
class OptimizationAlg
{
public:
    static constexpr size_t DEFAULT_MAX_ITERATIONS = 50;

    static constexpr size_t NUM_PARTICLES = __NUM_PARICLES;
    typedef __PARICLE_T particle_t;


    OptimizationAlg()
        : particles_{}
        , iteration_{ 0 }
        , maxIterations_{ DEFAULT_MAX_ITERATIONS }
        , seed_{ 0 }
    {
    }


    virtual ~OptimizationAlg() {}


    void run()
    {
        initializeParticles();

        for( uint64_t i = 0; i < maxIterations_; i++ )
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

    uint64_t iteration_;
    uint64_t maxIterations_;

    uint64_t seed_;


}; // class OptimizationAlg




#endif // OPTIMIZATIONALG_H