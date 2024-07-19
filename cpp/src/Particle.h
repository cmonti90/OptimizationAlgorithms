#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstddef>
#include <cstring>
#include <limits>


template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class BestParticle;

template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class Particle
{
public:

    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr size_t NUM_PARAMS = __NUM_PARAMS;

    inline Particle()
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memset( position_, 0, NUM_PARAMS * sizeof( param_t ) );
    }


    // Constructor and destructor
    inline Particle( const param_t position[NUM_PARAMS] )
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memcpy( position_, position, NUM_PARAMS * sizeof( param_t ) );
    }


    inline virtual ~Particle() {}

    // Copy constructor
    inline Particle( const Particle& other )
        : position_{}
        , fitness_{ other.fitness_ }
    {
        std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
    }


    // Assignment operator
    inline Particle& operator=( const Particle& other )
    {
        if ( this != &other )
        {
            std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
            fitness_ = other.fitness_;
        }

        return *this;
    }


    // Declare member functions here
    inline void getPosition( param_t position[NUM_PARAMS] ) const
    {
        std::memcpy( position, position_, NUM_PARAMS * sizeof( param_t ) );
    }


    inline void setPosition( const param_t position[NUM_PARAMS] )
    {
        std::memcpy( position_, position, NUM_PARAMS * sizeof( param_t ) );
    }


    inline fitness_t getFitness() const { return fitness_; };

    inline bool operator==( const Particle& other ) const { return fitness_ == other.fitness_; }
    inline bool operator!=( const Particle& other ) const { return fitness_ != other.fitness_; }
    inline bool operator>( const Particle& other ) const { return fitness_ > other.fitness_; }
    inline bool operator<( const Particle& other ) const { return fitness_ < other.fitness_; }
    inline bool operator>=( const Particle& other ) const { return fitness_ >= other.fitness_; }
    inline bool operator<=( const Particle& other ) const { return fitness_ <= other.fitness_; }

protected:

    param_t position_[NUM_PARAMS];
    fitness_t fitness_;

}; // class Particle


template< size_t __NUM_PARAMS, typename __PARAM_T, typename __FITNESS_T >
class BestParticle : protected Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    BestParticle()
        : Base{}
    {}

    virtual ~BestParticle() {}

    // const param_t* const getPosition() const { return &position_; }
    // const fitness_t* const getFitness() const { return &fitness_; }

    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;
    using Base::operator>;
    using Base::operator<;
    using Base::operator>=;
    using Base::operator<=;


    bool testParticle( const Base& particle )
    {
        if ( particle < *this )
        {
            *this = particle;
            return true;
        }

        return false;
    }


}; // class BestParticle

#endif // PARTICLE_H