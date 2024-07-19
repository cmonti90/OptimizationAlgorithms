#ifndef RNG_H
#define RNG_H

#include <random>

class Rng
{
public:
    Rng();
    ~Rng();

    double getUniform();
}; // class Rng

#endif // RNG_H