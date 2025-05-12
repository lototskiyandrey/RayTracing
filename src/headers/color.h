#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    if(linear_component > 0)
    {
        return std::sqrt(linear_component);
    }

    return 0;
}

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);

    int r_byte = int(255.999 * intensity.clamp(r));
    int g_byte = int(255.999 * intensity.clamp(g));
    int b_byte = int(255.999 * intensity.clamp(b));

    out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}

#endif