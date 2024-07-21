#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstddef>
#include <cstring>
#include <limits>


template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class Particle
{
public:

    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr size_t NUM_PARAMS = __NUM_PARAMS;

    // Constructor
    inline Particle()
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memset( position_, static_cast< param_t >( 0 ), NUM_PARAMS * sizeof( param_t ) );
    }


    // Destructor
    inline virtual ~Particle() {}


    // Copy Constructor
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


    inline void initialize( const param_t position[NUM_PARAMS] )
    {
        std::memcpy( position_, position, NUM_PARAMS * sizeof( param_t ) );
        fitness_ = std::numeric_limits< fitness_t >::max();
    }


    inline fitness_t getFitness() const { return fitness_; };

    inline bool operator==( const Particle& other ) const { return fitness_ == other.fitness_; }
    inline bool operator!=( const Particle& other ) const { return fitness_ != other.fitness_; }
    inline bool operator>( const Particle& other ) const { return fitness_ > other.fitness_; }
    inline bool operator<( const Particle& other ) const { return fitness_ < other.fitness_; }
    inline bool operator>=( const Particle& other ) const { return fitness_ >= other.fitness_; }
    inline bool operator<=( const Particle& other ) const { return fitness_ <= other.fitness_; }

    param_t position_[NUM_PARAMS];
    fitness_t fitness_;


}; // class Particle



template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class BestParticle : public Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >
{
public:

    using Base = Particle< __NUM_PARAMS, __PARAM_T, __FITNESS_T >;
    using param_t = __PARAM_T;
    using fitness_t = __FITNESS_T;

    BestParticle()
        : Base{}
    {}

    virtual ~BestParticle() {}

    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;
    using Base::operator>;
    using Base::operator<;
    using Base::operator>=;
    using Base::operator<=;


    bool trialParticle( const Base& particle )
    {
        if ( particle < *this )
        {
            *this = particle;
            return true;
        }

        return false;
    }


    template< size_t __NUM_PARTICLES >
    bool trialParticle( const Base ( &particles )[__NUM_PARTICLES] )
    {
        bool updated = false;

        for ( size_t i = 0; i < __NUM_PARTICLES; ++i )
        {
            if ( trialParticle( particles[i] ) )
            {
                updated = true;
            }
        }

        return updated;
    }


}; // class BestParticle

#endif // PARTICLE_H