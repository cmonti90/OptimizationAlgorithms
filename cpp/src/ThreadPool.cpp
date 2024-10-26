

#include "ThreadPool.h"


namespace MetaOpt
{


// Constructor
ThreadPool::ThreadPool( const int numThreads )
    : numThreads_{ numThreads }
    , workers_{}
    , tasks_{}
    , queue_mtx_{}
    , cv_{}
    , stop_{ true }
{
    workers_.resize( numThreads );
}


// Destructor
ThreadPool::~ThreadPool()
{
    stopThreads();
}


void ThreadPool::startThreads()
{
    std::unique_lock< std::mutex > lock( queue_mtx_ );

    if ( stop_ )
    {
        for ( size_t i{0}; i < numThreads_; ++i )
        {
            workers_.emplace_back( std::thread( [this]
            {
                while ( true )
                {
                    std::function< void() > task;

                    {
                        std::unique_lock< std::mutex > lock( this->queue_mtx_ );

                        this->cv_.wait( lock, [this] { return this->stop_ || !this->tasks_.empty(); } );

                        if ( this->stop_ && this->tasks_.empty() )
                        {
                            return;
                        }

                        task = std::move( this->tasks_.front() );

                        this->tasks_.pop();
                    }

                    task();
                }
            } ) );
        }

        stop_ = false;
    }
}


void ThreadPool::stopThreads()
{
    if ( !stop_ )
    {
        {
            std::unique_lock< std::mutex > lock( queue_mtx_ );
            stop_ = true;
        }

        cv_.notify_all();

        for ( std::thread& worker : workers_ )
        {
            if ( worker.joinable() )
            {
                worker.join();
            }
        }
    }
}

} // namespace MetaOpt

