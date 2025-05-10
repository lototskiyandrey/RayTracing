#ifndef RTWEEKEND_H
#define RTWEEKEND_H

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

// Basic Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

// Useful Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif