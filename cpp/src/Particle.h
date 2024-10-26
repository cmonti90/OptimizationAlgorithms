#ifndef PARTICLE_H
#define PARTICLE_H

#include "Rng.h"

#include <cstddef>
#include <cstring>
#include <limits>

#include <initializer_list>
#include <string>
#include <exception>

#include <iostream>


namespace MetaOpt
{

template< size_t __NUM_PARAMS, typename __PARAM_T = double, typename __FITNESS_T = double >
class Particle
{
public:

    using param_t   = __PARAM_T;
    using fitness_t = __FITNESS_T;

    static constexpr size_t NUM_PARAMS = __NUM_PARAMS;

    // Constructor
    inline Particle()
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memset( position_, 0, NUM_PARAMS * sizeof( param_t ) );
    }


    // Destructor
    inline virtual ~Particle() {}


    // Copy Constructor
    inline Particle( const Particle ( &other )[NUM_PARAMS] )
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        std::memcpy( position_, other, NUM_PARAMS * sizeof( param_t ) );
    }


    // Copy Constructor
    inline Particle( const Particle& other )
        : position_{}
        , fitness_{ other.fitness_ }
    {
        std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
    }


    // Copy Constructor
    inline Particle( const std::initializer_list< param_t >& other )
        : position_{}
        , fitness_{ std::numeric_limits< fitness_t >::max() }
    {
        if ( other.size() != NUM_PARAMS )
        {
            throw std::invalid_argument( "Particle::Particle: initializer list size must match number of parameters" );
        }

        std::memset( position_, 0, NUM_PARAMS * sizeof( param_t ) );
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


    // Addition operator
    inline Particle operator+( const Particle& other ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] + other.position_[i];
        }

        return result;
    }


    // Subtraction operator
    inline Particle operator-( const Particle& other ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] - other.position_[i];
        }

        return result;
    }


    // Multiplication operator
    inline Particle operator*( const Particle& other ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] * other.position_[i];
        }

        return result;
    }


    // Division operator
    inline Particle operator/( const Particle& other ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] / other.position_[i];
        }

        return result;
    }


    // Addition assignment operator
    inline Particle& operator+=( const Particle& other )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] += other.position_[i];
        }

        return *this;
    }


    // Subtraction assignment operator
    inline Particle& operator-=( const Particle& other )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] -= other.position_[i];
        }

        return *this;
    }


    // Multiplication assignment operator
    inline Particle& operator*=( const Particle& other )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] *= other.position_[i];
        }

        return *this;
    }


    // Division assignment operator
    inline Particle& operator/=( const Particle& other )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] /= other.position_[i];
        }

        return *this;
    }


    // Addition operator
    inline Particle operator+( const param_t scalar ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] + scalar;
        }

        return result;
    }


    // Subtraction operator
    inline Particle operator-( const param_t scalar ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] - scalar;
        }

        return result;
    }


    // Multiplication operator
    inline Particle operator*( const param_t scalar ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] * scalar;
        }

        return result;
    }


    // Division operator
    inline Particle operator/( const param_t scalar ) const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = position_[i] / scalar;
        }

        return result;
    }


    // Addition assignment operator
    inline Particle& operator+=( const param_t scalar )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] += scalar;
        }

        return *this;
    }


    // Subtraction assignment operator
    inline Particle& operator-=( const param_t scalar )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] -= scalar;
        }

        return *this;
    }


    // Multiplication assignment operator
    inline Particle& operator*=( const param_t scalar )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] *= scalar;
        }

        return *this;
    }


    // Division assignment operator
    inline Particle& operator/=( const param_t scalar )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] /= scalar;
        }

        return *this;
    }


    // Negation operator
    inline Particle operator-() const
    {
        Particle result;

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            result.position_[i] = -position_[i];
        }

        return result;
    }


    inline void initialize( const param_t lowerBounds[NUM_PARAMS], const param_t upperBounds[NUM_PARAMS] )
    {
        Rng<>* rng = Rng<>::getInstance();

        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            position_[i] = rng->drawUniform( lowerBounds[i], upperBounds[i] );
        }
    }


    inline void clip( const param_t lowerBounds[NUM_PARAMS], const param_t upperBounds[NUM_PARAMS] )
    {
        for ( size_t i = 0; i < NUM_PARAMS; ++i )
        {
            if ( position_[i] < lowerBounds[i] )
            {
                position_[i] = lowerBounds[i];
            }
            else if ( position_[i] > upperBounds[i] )
            {
                position_[i] = upperBounds[i];
            }
        }
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


// Particle addition operator
template< size_t NUM_PARAMS, typename PARAM_T, typename FITNESS_T >
inline Particle< NUM_PARAMS, PARAM_T, FITNESS_T > operator+( const PARAM_T scalar, const Particle< NUM_PARAMS, PARAM_T, FITNESS_T >& particle )
{
    return particle + scalar;
}


// Particle subtraction operator
template< size_t NUM_PARAMS, typename PARAM_T, typename FITNESS_T >
inline Particle< NUM_PARAMS, PARAM_T, FITNESS_T > operator-( const PARAM_T scalar, const Particle< NUM_PARAMS, PARAM_T, FITNESS_T >& particle )
{
    return particle - scalar;
}


// Particle multiplication operator
template< size_t NUM_PARAMS, typename PARAM_T, typename FITNESS_T >
inline Particle< NUM_PARAMS, PARAM_T, FITNESS_T > operator*( const PARAM_T scalar, const Particle< NUM_PARAMS, PARAM_T, FITNESS_T >& particle )
{
    return particle * scalar;
}


// Particle division operator
template< size_t NUM_PARAMS, typename PARAM_T, typename FITNESS_T >
inline Particle< NUM_PARAMS, PARAM_T, FITNESS_T > operator/( const PARAM_T scalar, const Particle< NUM_PARAMS, PARAM_T, FITNESS_T >& particle )
{
    return particle / scalar;
}



template< typename __PARTICLE_T >
class BestParticle : public __PARTICLE_T
{
public:

    using Base = __PARTICLE_T;


    inline BestParticle()
        : Base{}
    {}


    inline virtual ~BestParticle() {}


    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;
    using Base::operator>;
    using Base::operator<;
    using Base::operator>=;
    using Base::operator<=;


    inline bool trialParticle( const Base& particle )
    {
        if ( particle < *this )
        {
            *this = particle;
            return true;
        }

        return false;
    }


    template< size_t NUM_PARTICLES >
    inline bool trialParticle( const Base ( &particles )[NUM_PARTICLES] )
    {
        bool updated = false;

        for ( size_t i = 0; i < NUM_PARTICLES; ++i )
        {
            if ( trialParticle( particles[i] ) )
            {
                updated = true;
            }
        }

        return updated;
    }


}; // class BestParticle

} // namespace MetaOpt

#endif // PARTICLE_H