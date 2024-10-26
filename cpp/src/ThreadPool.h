#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <vector>


namespace MetaOpt
{

class ThreadPool
{
  public:

    ThreadPool() = delete;
    ThreadPool( const int numThreads );
    ~ThreadPool();

    void startThreads();
    void stopThreads();

    template< class F, class... Args >
    auto enqueue( F&& f, Args&& ...args ) -> std::future< typename std::result_of< F( Args... ) >::type >;


  private:

    int numThreads_;
    std::vector< std::thread > workers_;
    std::queue< std::function< void() > > tasks_;

    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool stop_;

}; // class ThreadPool


// Enqueue new work item
template< class F, class... Args >
auto ThreadPool::enqueue( F&& f, Args&& ... args ) -> std::future< typename std::result_of< F( Args... ) >::type >
{
    using return_type = typename std::result_of< F( Args... ) >::type;

    auto task = std::make_shared< std::packaged_task< return_type() > >(
                    std::bind( std::forward< F >( f ), std::forward< Args >( args )... )
                );

    std::future< return_type > res = task->get_future();

    {
        std::unique_lock< std::mutex > lock( queue_mtx_ );

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


} // namespace MetaOpt


#endif // THREAD_POOL_H