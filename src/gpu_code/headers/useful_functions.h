#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <fstream>
// #include <bits/stdc++.h>
#include <algorithm>
#include <functional>

// Namespaces
using std::make_shared;
using std::shared_ptr;

// Useful Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double e = 2.7182818284590452353;
const int hue_length = 360;

// Define Our Spectrum

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

inline double clamp(double a, double b, double c) 
{
    if(a < b)
    {
        return b;
    }
    if(a > c)
    {
        return c;
    }
    return a;
}

inline bool is_zero_vector(std::vector<double> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i) != 0.0)
        {
            return false;
        }
    }
    return true;
}

#endif