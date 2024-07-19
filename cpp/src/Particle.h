#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstddef>
#include <cstring>

template< size_t __NUM_PARAMS__, typename __PARAM_T__ = double, typename __FITNESS_T__ = double >
class Particle
{
public:

    static constexpr size_t NUM_PARAMS = __NUM_PARAMS__;
    typedef __PARAM_T__ param_t;
    typedef __FITNESS_T__ fitness_t;

    inline Particle()
        : position_{}
        , bestPosition_{}
        , bestFitness_{ 0.0 }
    {
        std::memset( position_, 0, NUM_PARAMS * sizeof( param_t ) );
        std::memset( bestPosition_, 0, NUM_PARAMS * sizeof( param_t ) );
    }


    // Constructor and destructor
    inline Particle( const param_t position[NUM_PARAMS] )
        : position_{}
        , bestPosition_{}
        , bestFitness_{ 0.0 }
    {
        std::memcpy( position_, position, NUM_PARAMS * sizeof( param_t ) );
        std::memcpy( bestPosition_, position, NUM_PARAMS * sizeof( param_t ) );
    }


    inline virtual ~Particle() {}

    // Copy constructor
    inline Particle( const Particle& other )
        : position_{}
        , bestPosition_{}
        , bestFitness_{ other.bestFitness_ }
    {
        std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
        std::memcpy( bestPosition_, other.bestPosition_, NUM_PARAMS * sizeof( param_t ) );
    }


    // Assignment operator
    inline Particle& operator=( const Particle& other )
    {
        if ( this != &other )
        {
            std::memcpy( position_, other.position_, NUM_PARAMS * sizeof( param_t ) );
            std::memcpy( bestPosition_, other.bestPosition_, NUM_PARAMS * sizeof( param_t ) );

            bestFitness_ = other.bestFitness_;
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


    inline void getBestPosition( param_t position[NUM_PARAMS] ) const
    {
        for ( size_t i = 0; i < NUM_PARAMS; i++ )
        {
            position[i] = bestPosition_[i];
        }
    }


    inline void setBestPosition( const param_t position[NUM_PARAMS] )
    {
        std::memcpy( bestPosition_, position, NUM_PARAMS * sizeof( param_t ) );
    }


    inline param_t getBestFitness() const { return bestFitness_; }
    inline void setBestFitness( param_t fitness ) { bestFitness_ = fitness; }

protected:
    // Declare member variables here

    param_t position_[NUM_PARAMS];

    param_t bestPosition_[NUM_PARAMS];
    fitness_t bestFitness_;

}; // class Particle

#endif // PARTICLE_H