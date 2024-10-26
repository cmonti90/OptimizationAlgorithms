#ifndef TIMER_H
#define TIMER_H


#include <chrono>

namespace MetaOpt
{
class Timer
{
public:

    static Timer* getInstance()
    {
        static Timer timer;

        return &timer; 
    }

    ~Timer() = default;

    void printTimeStats() const;

    void startInitializeParticleLoop();
    void startUpdateParticleLoop();
    void startEvaluateParticleLoop();

    void stopInitializeParticleLoop();
    void stopUpdateParticleLoop();
    void stopEvaluateParticleLoop();

private:

    template< typename T > using dur = std::chrono::duration< T >;
    using time_point = std::chrono::_V2::system_clock::time_point;

    Timer() = default;

    time_point initializeParticleLoopStamp_;
    time_point updateParticleLoopStamp_;
    time_point evaluateParticleLoopStamp_;


    dur< double > initializeParticleLoop_;
    dur< double > updateParticleLoop_;
    dur< double > evaluateParticleLoop_;

}; // class Timer

} // namespace MetaOpt

#endif // TIMER_H