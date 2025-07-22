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


vector<int> convert_rgb_to_hsb(std::vector<int> rgb)
{   
    int red = rgb.at(0);
    int green = rgb.at(1);
    int blue = rgb.at(2);
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
        saturation = 100 * delta/C_max;
    }

    double brightness = 100 * C_max;

    std::vector<int> hsb_color = {(int)hue, (int)saturation, (int)brightness};

    return hsb_color;
}

vector<int> convert_hsb_to_rgb(vector<int> hsb)
{
    // Uses the formula found here: https://www.rapidtables.com/convert/color/hsv-to-rgb.html
    int hue = hsb.at(0);
    double saturation = (double)hsb.at(1) / 100;
    double brightness = (double)hsb.at(2) / 100;

    double C = brightness * saturation;
    double X = C * (1 - std::fabs(((hue / 60) % 2) - 1));
    double m = brightness - C;

    double red_prime, green_prime, blue_prime;

    if(0 <= hue && hue < 60)
    {
        red_prime = C;
        green_prime = X;
        blue_prime = 0.0;
    }
    else if(60 <= hue && hue < 120)
    {
        red_prime = X;
        green_prime = C;
        blue_prime = 0.0;
    }
    else if(120 <= hue && hue < 180)
    {
        red_prime = 0.0;
        green_prime = C;
        blue_prime = X;
    }
    else if(180 <= hue && hue < 240)
    {
        red_prime = 0.0;
        green_prime = X;
        blue_prime = C;
    }
    else if(240 <= hue && hue < 300)
    {
        red_prime = X;
        green_prime = 0.0;
        blue_prime = C;
    }
    else if(300 <= hue && hue < 360)
    {
        red_prime = C;
        green_prime = 0.0;
        blue_prime = X;
    }

    int red = (int)((red_prime+m)*255);
    int green = (int)((green_prime+m)*255);
    int blue = (int)((blue_prime+m)*255);

    std::vector<int> rgb = {red, green, blue};

    return rgb;
}