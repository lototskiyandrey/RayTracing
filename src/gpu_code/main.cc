#include <bits/stdc++.h>
#include "headers/interval.h"
#include "headers/useful_functions.h"
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/hittable_list.h"
#include "headers/hittable.h"
#include "headers/material.h"


vec3 sample_square();
std::vector<int> get_sample_pixel_color(const ray &r, const hittable &world, int scatter_depth);
std::vector<double> multiply_two_vectors(std::vector<double> vec1, std::vector<double> vec2);
std::vector<double> multiply_vector_by_constant(std::vector<double> vec, double f);
std::vector<int> convert_hsb_distribution_into_rgb(std::vector<double> hsb_spectrum);
std::vector<int> sum_two_vectors(std::vector<int> vec1, std::vector<int> vec2);
std::vector<int> divide_vector_by_constant(std::vector<int> vec, int d);
std::vector<int> convert_rgb_to_hsb(std::vector<int> rgb);
std::vector<int> convert_hsb_to_rgb(vector<int> hsb);

int main(int argc, char **argv) 
{

    hittable_list world;

    std::vector<double> yellow_color(hue_length, 0.0);
    for(int i = 50; i <= 65; i++)
    {
        yellow_color.at(i) = 1.0 / (65 - 15);
    }

    material yellow(0.9, yellow_color, false);

    std::vector<double> white_color(hue_length, 0.0);
    for(int i = 0; i < hue_length; i++)
    {
        white_color.at(i) = 10.0 / (hue_length);
    }

    material white(7.0, white_color, false);

    world.add(make_shared<sphere>(vec3(0,0,-1), 0.5, yellow));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100, white));

    int image_width = 700;
    int image_height = 500;

    vec3 center = vec3(0,0,0);
    vec3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    int samples_per_pixel = 100;
    int scatter_depth = 5;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int j = 0; j < image_height; j++)
    {   
        std::clog << "\rScanline: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {
            // ray r(center, ray_direction);

            std::vector<int> pixel_color(3, 0);

            for(int sample = 0; sample < samples_per_pixel; sample++)
            {
                auto offset = sample_square();
                auto pixel_sample = pixel00_loc + ((i+offset.x()) * pixel_delta_u) + ((j+offset.y()) * pixel_delta_v);
                auto ray_direction = pixel_sample - center;

                ray r(center, ray_direction);
                
                std::vector<int> pixel_color_sample = get_sample_pixel_color(r, world, scatter_depth);

                pixel_color = sum_two_vectors(pixel_color, pixel_color_sample);
            }

            pixel_color = divide_vector_by_constant(pixel_color, samples_per_pixel);

            std::cout << pixel_color.at(0) << ' ' << pixel_color.at(1) << ' ' << pixel_color.at(2) << "\n";
        }
    }
    std::clog << "\rFinished!              \n";
    return 0;
}

// int main(int argc, char **argv)
// {
//     // std::vector<int> rgb = {237, 188, 0};
//     // std::vector<int> hsb = convert_rgb_to_hsb(rgb);
//     // std::clog << hsb.at(0) << ' ' << hsb.at(1) << ' ' << hsb.at(2) << std::endl;
//     // std::vector<int> hsb = {48, 100, 93};
//     // std::vector<int> rgb = convert_hsb_to_rgb(hsb);
//     // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
//     // std::vector<int> vec1 = {1, 4, 8};
//     // std::vector<int> vec2 = {9, 8, 11};
//     // std::clog << vec1.at(0) << ' ' << vec1.at(1) << ' ' << vec1.at(2) << std::endl;
//     // std::clog << vec2.at(0) << ' ' << vec2.at(1) << ' ' << vec2.at(2) << std::endl;
//     // vec1 = sum_two_vectors(vec1, vec2);
//     // std::clog << vec1.at(0) << ' ' << vec1.at(1) << ' ' << vec1.at(2) << std::endl;
//     std::vector<double> yellow_color(hue_length, 0.0);
//     for(int i = 50; i <= 65; i++)
//     {
//         yellow_color.at(i) = 724.0 / (65 - 50);
//         // std::clog << yellow_color.at(i) << std::endl;
//     }
//     // std::vector<double> white_color(hue_length, 0.0);
//     // for(int i = 0; i < hue_length; i++)
//     // {
//     //     white_color.at(i) = 724.0 / (hue_length);
//     //     // std::clog << white_color.at(i) << std::endl;
//     // }
//     std::vector<int> rgb = convert_hsb_distribution_into_rgb(yellow_color);
//     std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
// }


std::vector<int> convert_rgb_to_hsb(std::vector<int> rgb)
{   
    int red = rgb.at(0);
    int green = rgb.at(1);
    int blue = rgb.at(2);
    // Uses the formula found here: https://www.rapidtables.com/convert/color/rgb-to-hsv.html
    double red_prime = (double)red / 255;
    double green_prime = (double)green / 255;
    double blue_prime = (double)blue / 255;

    double C_max = std::fmax(std::fmax(red_prime, green_prime), blue_prime);
    double C_min = std::fmin(std::fmin(red_prime, green_prime), blue_prime);

    double delta = C_max - C_min;

    double hue;

    std::clog << "Delta: " << delta << std::endl;

    if(delta == 0.0)
    {
        hue = 0;
    }
    else if(C_max == red_prime) 
    {
        hue = 60 * ((int)((green_prime-blue_prime)/delta) % 6 + ( ((green_prime-blue_prime)/delta) - (int)((green_prime-blue_prime)/delta) ));
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

std::vector<int> convert_hsb_to_rgb(vector<int> hsb)
{
    // Uses the formula found here: https://www.rapidtables.com/convert/color/hsv-to-rgb.html
    int hue = hsb.at(0);
    double saturation = (double)hsb.at(1) / 100;
    double brightness = (double)hsb.at(2) / 100;

    double C = brightness * saturation;
    double X = C * (1 - std::fabs(((hue / 60) % 2) - 1 + (((double)hue / 60) - (hue/60)) ));
    // std::clog << X << std::endl;
    // std::clog << C << std::endl;
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

vec3 sample_square() 
{
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

std::vector<int> get_sample_pixel_color(const ray &r, const hittable &world, int scatter_depth)
{   

    ray r_copy = ray(r.orig(), r.dir());

    std::vector<double> color_distribution(hue_length, 0.0);

    for(int i = 0; i < scatter_depth; i++)
    {   
        hit_record rec;
        if(!world.hit(r_copy, interval(0, infinity), rec))
        {
            return std::vector<int>(3, 0);
        }

        double albedo = rec.mat.albedo;
        std::vector<double> color = rec.mat.color;
        bool is_refractive = rec.mat.is_refractive;

        if(!is_refractive)
        {
            if(is_zero_vector(color_distribution))
            {
                color_distribution = color;
                color_distribution = multiply_vector_by_constant(color_distribution, albedo);  
            }
            else  
            {   
                color = multiply_vector_by_constant(color, albedo);
                // std::vector<int> rgb = convert_hsb_distribution_into_rgb(color);
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                // rgb = convert_hsb_distribution_into_rgb(color_distribution);
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                color_distribution = multiply_two_vectors(color_distribution, color);
                // rgb = convert_hsb_distribution_into_rgb(color_distribution);
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                // std::clog << albedo << std::endl;
                // exit(0);
            }
            // bool should_exit = false;
            if(albedo >= 1.0)
            {
                // std::vector<int> rgb = convert_hsb_distribution_into_rgb(color);
                // std::clog << std::endl;
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                // rgb = convert_hsb_distribution_into_rgb(color_distribution);
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                // rgb = convert_hsb_distribution_into_rgb(color_distribution);
                // std::clog << rgb.at(0) << ' ' << rgb.at(1) << ' ' << rgb.at(2) << std::endl;
                // std::clog << albedo << std::endl;
                // std::clog << i << std::endl;
                // std::clog << r_copy.dir() << ' ' << r_copy.orig() << std::endl;
                // std::clog << r.dir() << ' ' << r.orig() << std::endl;
                // exit(0);
                // should_exit = true;
                // if(i == 0)
                // {
                //     color_distribution = multiply_vector_by_constant(color_distribution, 1.0/100.0);
                // }
                break;
            }

            vec3 direction = random_on_hemisphere(rec.normal);
            r_copy = ray(rec.p, direction);
        }
    }

    // Convert the hue distribution into rgb
    return convert_hsb_distribution_into_rgb(color_distribution);
}

std::vector<int> convert_hsb_distribution_into_rgb(std::vector<double> hsb_spectrum)
{
    std::vector<int> rgb(3, 0);
    int num_non_zero = 0;
    for(int i = 0; i < hsb_spectrum.size(); i++)
    {
        int hue = i;
        int saturation = 100;
        int brightness = (int)(hsb_spectrum.at(i) * 100);

        num_non_zero = brightness != 0 ? num_non_zero+1 : num_non_zero;

        std::vector<int> hsb = {hue, saturation, brightness};

        std::vector<int> rgb_temp = convert_hsb_to_rgb(hsb);

        rgb = sum_two_vectors(rgb, rgb_temp);
    }
    // if(num_non_zero > 0)
    // {
    //     rgb = divide_vector_by_constant(rgb, num_non_zero);
    // }
    return rgb;
}

std::vector<double> multiply_two_vectors(std::vector<double> vec1, std::vector<double> vec2)
{
    // Assumes that the two vectors have the same length.
    std::vector<double> vec3(vec1.size(), 0.0);
    for(int i = 0; i < vec1.size(); i++)
    {
        vec3.at(i) = vec1.at(i) * vec2.at(i);
    }
    return vec3;
}

std::vector<double> multiply_vector_by_constant(std::vector<double> vec, double f)
{
    for(int i = 0; i < vec.size(); i++)
    {
        vec.at(i) *= f;
    }
    return vec;
}

std::vector<int> sum_two_vectors(std::vector<int> vec1, std::vector<int> vec2)
{
    std::vector<int> vec3(vec1.size(), 0);
    for(int i = 0; i < vec1.size(); i++)
    {
        vec3.at(i) = vec1.at(i) + vec2.at(i);
    }
    return vec3;
}

std::vector<int> divide_vector_by_constant(std::vector<int> vec, int d)
{
    for(int i = 0; i < vec.size(); i++)
    {
        vec.at(i) /= d;
    }
    return vec;
}