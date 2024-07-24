#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <stdexcept>


class Semaphore
{
public:

    explicit Semaphore( const int count )
        : count_( count )
        , max_count_( count )
        , mtx_()
        , cv_()
    {
        if ( count <= 0 ) {
            throw std::invalid_argument( "Semaphore initial count must be positive" );
        }
    }


    void acquire()
    {
        std::unique_lock<std::mutex> lock( mtx_ );
        cv_.wait( lock, [this]() { return count_ > 0; } );
        --count_;
    }


    void release()
    {
        std::unique_lock<std::mutex> lock( mtx_ );
        ++count_;
        cv_.notify_one();
    }


private:

    int count_;
    const int max_count_;

    std::mutex mtx_;
    std::condition_variable cv_;
};

#endif // SEMAPHORE_H