#include <bits/stdc++.h>
#include "headers/interval.h"
#include "headers/useful_functions.h"
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/hittable_list.h"
#include "headers/hittable.h"
#include "headers/material.h"

void write_color(std::ostream &out, std::vector<double> spectrum);

void generate_spectrum_background(std::vector<double> &spectrum, const ray r);
void generate_black_background(std::vector<double> &spectrum);
// double hit_sphere(vec3 center, double radius, ray r);
std::vector<double> sum_two_vectors(std::vector<double> vec1, std::vector<double> vec2);
vec3 sample_square();
ray get_ray(int i, int j);

void spectrum_color(const ray &r, const hittable &world, std::vector<double> &spectrum, int scatter_depth);

int main() 
{   
    // Image
    int image_width = 700;
    int image_height = 500;


    // Materials

    std::vector<double> white(spectrum_length);
    std::vector<double> yellow(spectrum_length);
    generate_spectrum_gaussian(white, 450, 100);
    generate_spectrum_gaussian(yellow, 575, 20);

    material white_sphere(0.9, white);
    material yellow_light_sphere(1.5, yellow);

    // World

    hittable_list world;

    world.add(std::make_shared<sphere>(vec3(0,0,-1), 0.5, white_sphere));
    world.add(std::make_shared<sphere>(vec3(0,-100.5,-1),100, yellow_light_sphere));

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

    int samples_per_pixel = 100;

    for(int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {   

            std::vector<double> temp_spectrum(spectrum_length);
            for(int sample = 0; sample < samples_per_pixel; sample++)
            {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                ray r = get_ray(i,j);
            
                spectrum_color(r, world, temp_spectrum, 5);

                spectrum = sum_two_vectors(temp_spectrum, spectrum);
            }
            
            spectrum = multiply_vector_by_constant(spectrum, 1.0 / samples_per_pixel);

            write_color(std::cout, spectrum);
        }
    }

    std::clog << "\rDone.                             \n";


    

    return 0;
}

ray get_ray(int i, int j) 
{   
    auto center = vec3(0,0,0);

    // Camera
    int image_width = 700;
    int image_height = 500;
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

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 sample_square() 
{
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}



/*

Implmenting a more realistic color system. Every ray is now an array where each element corresponds to the intesity of
a particular wavelength of visible light

*/
 
vec3 unit_random_on_hemisphere(vec3 normal)
{
    vec3 vec = random_unit_vector();
    while(dot(vec, normal) < 0.0)
    {
        vec = random_unit_vector();
    }

    return vec;
}

void spectrum_color(const ray &r, const hittable &world, std::vector<double> &spectrum, int scatter_depth)
{   

    spectrum = create_zero_vector(spectrum_length);

    ray r_copy(r.orig(), r.dir());

    for(int i = 0; i < scatter_depth; i++)
    {
       hit_record rec; 

        if(!world.hit(r_copy, interval(0, infinity), rec))
        {
            spectrum = create_zero_vector(spectrum_length);
            break;
        }

        vec3 normal = rec.normal;
        double albedo = rec.mat.albedo;
        std::vector<double> color = rec.mat.color;

        if(is_zero_vector(spectrum))
        {
            spectrum = color;
            spectrum = multiply_vector_by_constant(spectrum, albedo);
        }
        else  
        {
            color = multiply_vector_by_constant(spectrum, albedo);
            spectrum = sum_two_vectors(spectrum, color);
        }

        if(albedo > 1.0)
        {
            break;
        }

        vec3 new_dir = unit_random_on_hemisphere(rec.normal);
        r_copy = ray(rec.p, new_dir);
    }


    // hit_record rec;
    // if(world.hit(r, interval(0, infinity), rec))
    // {
    //     vec3 normal = rec.normal;
    //     generate_spectrum_gaussian(spectrum, 450 * (1.5 - std::fabs(normal.x())/2.0), 30);
    //     return;
    // }
    // // generate_spectrum_background(spectrum, r);
    // generate_black_background(spectrum);
    return;
}

std::vector<double> sum_two_vectors(std::vector<double> vec1, std::vector<double> vec2)
{
    std::vector<double> vec_result(spectrum_length);

    for(int i = 0; i < vec_result.size(); i++)
    {
        vec_result[i] = vec1[i] + vec2[i];
    }

    return vec_result;
}





void generate_black_background(std::vector<double> &spectrum) 
{
    for(int i = 0; i < spectrum.size(); i++)
    {
        spectrum[i] = 0.0;
    }
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



