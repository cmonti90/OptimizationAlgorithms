

#include "Semaphore.h"


namespace MetaOpt
{

Semaphore::Semaphore( const int count )
    : count_( count )
    , max_count_( count )
    , mtx_()
    , cv_()
{
    if ( count <= 0 ) {
        throw std::invalid_argument( "Semaphore initial count must be positive" );
    }
}


void Semaphore::acquire()
{
    std::unique_lock< std::mutex > lock( mtx_ );
    cv_.wait( lock, [this]() { return count_ > 0; } );
    --count_;
}


void Semaphore::release()
{
    std::unique_lock< std::mutex > lock( mtx_ );
    ++count_;
    cv_.notify_one();
}


} // namespace MetaOpt
