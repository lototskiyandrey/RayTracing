#include <bits/stdc++.h>
#include "headers/useful_functions.h"
#include "headers/vec3.h"
#include "headers/ray.h"

void write_color(std::ostream &out, std::vector<double> spectrum);

void generate_spectrum_background(std::vector<double> &spectrum, const ray r);
bool hit_sphere(vec3 center, double radius, ray r);

int main() 
{   
    // Image
    int image_width = 700;
    int image_height = 500;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = vec3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    std::vector<double> spectrum(spectrum_length);
    load_and_populate_cmfs(cmf_r, cmf_g, cmf_b);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {   

            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            if(hit_sphere(vec3(0,0,-1.5), 0.5, r))
            {
                generate_spectrum_gaussian(spectrum, 680, 30);
            }
            else  
            {
                generate_spectrum_background(spectrum, r);
            }

            write_color(std::cout, spectrum);
        }
    }

    std::clog << "\rDone.                             \n";


    

    return 0;
}

/*

Implmenting a more realistic color system. Every ray is now an array where each element corresponds to the intesity of
a particular wavelength of visible light

*/


void generate_spectrum_background(std::vector<double> &spectrum, const ray r)
{
    auto a = r.dir().y();

    int variance = 50;

    std::vector<double> f(spectrum_length);
    std::vector<double> s(spectrum_length);
    generate_spectrum_gaussian(f, 460, variance);
    flatten_spectrum(f, 0.5);
    generate_spectrum_uniform(s, 0, spectrum_length);
    flatten_spectrum(s, 1.4);
    superposition_spectrum(spectrum, f, s);
}


bool hit_sphere(vec3 center, double radius, ray r)
{
    double a = dot(r.dir(), r.dir());
    double b = dot(-2*r.dir(), center - r.orig());
    double c = dot(center - r.orig(), center - r.orig()) - (radius * radius);

    double determinant = b*b - 4*a*c;

    return determinant >= 0;
}



void write_color(std::ostream &out, std::vector<double> spectrum)
{   
    // tristimulus values
    double X = integrate_spectrum_cmf(spectrum, cmf_r);
    double Y = integrate_spectrum_cmf(spectrum, cmf_g);
    double Z = integrate_spectrum_cmf(spectrum, cmf_b);

    // linear RGB
    double r_lin = (3.2406 * X) + (-1.5372 * Y) + (-0.4986 * Z);
    double g_lin = (-0.9689 * X) + (1.8758 * Y) + (0.0415 * Z);
    double b_lin = (0.0557 * X) + (-0.2040 * Y) + (1.0570 * Z);

    // Gamma correction
    double r_lin_corrected = r_lin <= 0.0031308 ? (12.92 * r_lin) : (1.055 * std::pow(r_lin, 1.0/2.4) - 0.055);
    double g_lin_corrected = g_lin <= 0.0031308 ? (12.92 * g_lin) : (1.055 * std::pow(g_lin, 1.0/2.4) - 0.055);
    double b_lin_corrected = b_lin <= 0.0031308 ? (12.92 * b_lin) : (1.055 * std::pow(b_lin, 1.0/2.4) - 0.055);

    // Clamping to [0,1]
    double r_clamped = clamp(r_lin_corrected, 0.0, 1.0);
    double g_clamped = clamp(g_lin_corrected, 0.0, 1.0);
    double b_clamped = clamp(b_lin_corrected, 0.0, 1.0);

    // Set to values between 0 and 255.
    int r_byte = int(255 * r_clamped);
    int g_byte = int(255 * g_clamped);
    int b_byte = int(255 * b_clamped);

    // Print out
    out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}



