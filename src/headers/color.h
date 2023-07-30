#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "camera.h"

#include <iostream>
#include <tuple>
#include <vector>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number fo samples and gamma correct for gamma=2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' ' 
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

std::vector<std::vector<std::tuple<int, int, int>>> write_color_to_array(color pixel_color, int samples_per_pixel, std::vector<std::vector<std::tuple<int, int, int>>> &currentImg, int i, int j) {

    std::tuple<int, int ,int> pixel;

    // Function is the same as write color function, with only minor difference in how we output.

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number fo samples and gamma correct for gamma=2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    pixel = std::make_tuple( static_cast<int>(256 * clamp(r, 0.0, 0.999)), static_cast<int>(256 * clamp(g, 0.0, 0.999)), static_cast<int>(256 * clamp(b, 0.0, 0.999)) );

    currentImg[j][i] = pixel;

    return currentImg;
}

#endif