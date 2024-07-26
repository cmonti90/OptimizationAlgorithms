
#include "Timer.h"

#include <iostream>


void Timer::printTimeStats() const
{
    std::cout << "initializeParticleLoop: " << initializeParticleLoop_.count() << " sec" << std::endl;
    std::cout << "udpateParticleLoop    : " << updateParticleLoop_.count() << " sec" << std::endl;
    std::cout << "evaluateParticleLoop  : " << evaluateParticleLoop_.count() << " sec" << std::endl;
}



void Timer::startInitializeParticleLoop()
{
    initializeParticleLoopStamp_ = std::chrono::high_resolution_clock::now();
}


void Timer::startUpdateParticleLoop()
{
    updateParticleLoopStamp_ = std::chrono::high_resolution_clock::now();
}


void Timer::startEvaluateParticleLoop()
{
    evaluateParticleLoopStamp_ = std::chrono::high_resolution_clock::now();
}


void Timer::stopInitializeParticleLoop()
{
    const time_point stopTime = std::chrono::high_resolution_clock::now();

    initializeParticleLoop_ += stopTime - initializeParticleLoopStamp_;
}


void Timer::stopUpdateParticleLoop()
{
    const time_point stopTime = std::chrono::high_resolution_clock::now();

    updateParticleLoop_ += stopTime - updateParticleLoopStamp_;
}


void Timer::stopEvaluateParticleLoop()
{
    const time_point stopTime = std::chrono::high_resolution_clock::now();

    evaluateParticleLoop_ += stopTime - evaluateParticleLoopStamp_;
}

