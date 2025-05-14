#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Namespaces
using std::make_shared;
using std::shared_ptr;

// Useful Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

// Return a random real in [0,1)
inline double random_double() 
{
    return std::rand() / (RAND_MAX + 1.0);
}

// Return a random real in [min, max)
inline double random_double(double min, double max) 
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return int(random_double(min, max+1));
}

#endif