#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>


template<size_t NumThreads>
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue( F&& f, Args&& ... args ) -> std::future<typename std::result_of<F( Args... )>::type>;

private:
    std::array<std::thread, NumThreads> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool stop_;
};


// Constructor
template< size_t NumThreads >
ThreadPool<NumThreads>::ThreadPool()
    : workers_{}
    , tasks_{}
    , queue_mtx_{}
    , cv_{}
    , stop_{ false }
{
    for ( auto& worker : workers_ )
    {
        worker = std::thread( [this] {
            while ( true )
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock( this->queue_mtx_ );
                    
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
        } );
    }
}


// Destructor
template<size_t NumThreads>
ThreadPool<NumThreads>::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock( queue_mtx_ );
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


// Enqueue new work item
template<size_t NumThreads>
template<class F, class... Args>
auto ThreadPool<NumThreads>::enqueue( F&& f, Args&& ... args ) -> std::future<typename std::result_of<F( Args... )>::type>
{
    using return_type = typename std::result_of<F( Args... )>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
                    std::bind( std::forward<F>( f ), std::forward<Args>( args )... )
                );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock( queue_mtx_ );

        // Don't allow enqueueing after stopping the pool
        if ( stop_ )
        {
            throw std::runtime_error( "enqueue on stopped ThreadPool" );
        }

        tasks_.emplace( [task]() { ( *task )(); } );
    }

    cv_.notify_one();

    return res;
}


#endif // THREAD_POOL_H