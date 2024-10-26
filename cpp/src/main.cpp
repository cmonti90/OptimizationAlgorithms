

#include "SwarmOptimization.h"
#include "DifferentialEvolution.h"

#include "Timer.h"

#include <chrono>
#include <iostream>



double fitnessFunc( const MetaOpt::Particle< 2 >& p )
{
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

    return p.position_[0] * p.position_[0] + p.position_[1] * p.position_[1];
}


int main()
{
    const double lowerBound[2] = {-5.0, -5.0};
    const double upperBound[2] = {5.0, 5.0};

    // MetaOpt::SwarmOptimization< 30, 2 > oa{ lowerBound, upperBound, 8 };
    MetaOpt::DifferentialEvolution< 30, 2 > oa{ lowerBound, upperBound, 8 };

    oa.setFitnessFunc( fitnessFunc );

    oa.setMaxIterations( 50 );

    auto start = std::chrono::high_resolution_clock::now();
    oa.run();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration< double > duration = end - start;

    std::cout << "The function took " << duration.count() << " seconds to run." << std::endl;

    MetaOpt::Timer::getInstance()->printTimeStats();

    return 0;
}