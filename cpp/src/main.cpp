
#include "Thread.h"
#include "Particle.h"
#include "SwarmParticle.h"
// #include "SwarmOptimization.h"

#include "Rng.h"

#include <iostream>

#include <thread>

int main()
{
    const double lowerBound[3] = {1.0, 2.0, 3.0};
    const double upperBound[3] = {2.0, 3.0, 4.0};

    Particle< 3 > p;

    for ( size_t i = 0; i < 3; i++ )
    {
        std::cout << p.position_[i] << std::endl;
    }

    SwarmParticle< 3 > sp;

    for ( size_t i = 0; i < 3; i++ )
    {
        std::cout << sp.position_[i] << std::endl;
    }

    Rng< 3 >* rng = Rng< 3 >::getInstance();

    // SwarmOptimization< 20, 2 > oa;

    return 0;
}