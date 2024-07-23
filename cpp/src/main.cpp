
#include "Thread.h"
#include "Particle.h"
#include "SwarmParticle.h"
#include "SwarmOptimization.h"

#include "Rng.h"

#include <iostream>

#include <thread>

// double fitnessFunc( const Particle< 2 >& p )
// {
//     return p.position_[0] * p.position_[0] + p.position_[1] * p.position_[1];
// }


int main()
{
    const double lowerBound[2] = {-5.0, -5.0};
    const double upperBound[2] = {5.0, 5.0};

    SwarmOptimization< 50, 2 > oa{ lowerBound, upperBound };

    std::function< double( const Particle< 2 >& ) > fitnessFunc = []( const Particle< 2 >& p )
    {
        return ( p.position_[0] + 1.0 ) * ( p.position_[0] + 1.0 ) + p.position_[1] * p.position_[1];
    };

    oa.setFitnessFunc( fitnessFunc );

    oa.setMaxIterations( 10000 );

    oa.run();

    return 0;
}