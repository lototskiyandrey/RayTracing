#include <bits/stdc++.h>
#include "headers/interval.h"
#include "headers/useful_functions.h"
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/hittable_list.h"
#include "headers/hittable.h"
#include "headers/material.h"


void spectrum_color(const ray &r, const hittable &world, std::vector<double> &spectrum, int scatter_depth);

int main(int argc, char **argv) 
{

    int image_width = 700;
    int image_height = 500;

    vec3 center;
    vec3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    auto focal_length = 1.0;

    auto viewport_u = vec3(image_width, 0, 0);
    auto viewport_v = vec3(0, -image_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    for(int j = 0; j < image_height; j++)
    {   
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        std::clog << "\rScanline: " << image_height - j << std::flush;
        for(int i = 0; i < image_width; i++)
        {

        }
    }


    return 0;
}


vector<int> convert_rgb_to_hsb(int red,int green,int blue)
{
    // Uses the formula found here: https://www.rapidtables.com/convert/color/rgb-to-hsv.html
    double red_prime = (double)red / 255;
    double green_prime = (double)green / 255;
    double blue_prime = (double)blue / 255;

    double C_max = std::fmax(std::fmax(red_prime, green_prime), blue_prime);
    double C_min = std::fmax(std::fmin(red_prime, green_prime), blue_prime);

    double delta = C_max - C_min;

    double hue;

    if(delta == 0)
    {
        hue = 0;
    }
    else if(C_max == red_prime) 
    {
        hue = 60 * ((int)((green_prime-blue_prime)/delta) % 6);
    }
    else if(C_max == green_prime)
    {
        hue = 60 * ((blue_prime-red_prime)/delta + 2);
    }
    else if(C_max == blue_prime) 
    {
        hue = 60 * ((red_prime-green_prime)/delta + 4);
    }

    double saturation;
    
    if(C_max == 0)
    {
        saturation = 0;
    }
    else  
    {
        saturation = delta/C_max;
    }

    double brightness = C_max;

    std::vector<int> hsb_color = {(int)hue, (int)saturation, (int)brightness};

    return hsb_color;
}






