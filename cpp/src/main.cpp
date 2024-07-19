
#include "Thread.h"
#include "Particle.h"
#include "SwarmParticle.h"

#include <iostream>

#include <thread>

int main()
{

    const double position1[3] = {1.0, 2.0, 3.0};
    Particle<3> p( position1 );

    double position[3];

    p.getPosition( position );

    for ( size_t i = 0; i < 3; i++ )
    {
        std::cout << position[i] << std::endl;
    }

    SwarmParticle<3> sp;

    sp.getPosition( position );

    for ( size_t i = 0; i < 3; i++ )
    {
        std::cout << position[i] << std::endl;
    }

    return 0;
}