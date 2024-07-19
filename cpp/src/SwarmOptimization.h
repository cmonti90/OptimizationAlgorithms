#ifndef SWARM_OPTIMIZATION_H
#define SWARM_OPTIMIZATION_H

#include "OptimizationAlg.cpp"


template< size_t __NUM_PARTICLES__, typename __PARTICLE_T__ >
class SwarmOptimization : public OptimizationAlg< __NUM_PARTICLES__, __PARTICLE_T__ >
{
public:
    // Constructor
    SwarmOptimization();

    // Destructor
    ~SwarmOptimization();

    // Public member functions

private:
    // Private member variables

    // Private member functions
}; // class SwarmOptimization


#endif // SWARM_OPTIMIZATION_H