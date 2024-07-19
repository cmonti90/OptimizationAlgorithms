#ifndef SWARM_OPTIMIZATION_H
#define SWARM_OPTIMIZATION_H

#include "OptimizationAlg.cpp"


template< size_t __NUM_PARTICLES, typename __PARTICLE_T >
class SwarmOptimization : public OptimizationAlg< __NUM_PARTICLES, __PARTICLE_T >
{
public:
    // Constructor
    SwarmOptimization();

    // Destructor
    virtual ~SwarmOptimization();

    // Public member functions

private:
    // Private member variables

    // Private member functions
}; // class SwarmOptimization


#endif // SWARM_OPTIMIZATION_H