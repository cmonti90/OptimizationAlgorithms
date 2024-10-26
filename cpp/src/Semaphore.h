#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <stdexcept>



namespace MetaOpt
{

class Semaphore
{
public:

    explicit Semaphore( const int count );
    ~Semaphore() = default;


    void acquire();
    void release();


private:

    int count_;
    const int max_count_;

    std::mutex mtx_;
    std::condition_variable cv_;

    Semaphore() = delete;
    Semaphore( const Semaphore& ) = delete;
    Semaphore& operator=( const Semaphore& ) = delete;

}; // class Semaphore

} // namespace MetaOpt

#endif // SEMAPHORE_H