#include "./headers/rtweekend.h"


#include "./headers/color.h"
#include "./headers/ray.h"
#include "./headers/hittable_list.h"
#include "./headers/vec3.h"
#include "./headers/sphere.h"
#include "./headers/camera.h"
#include "./headers/material.h"

#include <iostream>
#include <memory>
#include <omp.h>


#define THREAD_NUM 12 // 12 threads on a ryzn 5600x processor.

color ray_color(const ray &r, const hittable &world, int depth);

hittable_list random_scene();

hittable_list simpler_random_scene();

void generate_image(const std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width);

void renderImage(std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width, hittable_list world, int samples_per_pixel, camera cam, int max_depth);

int main(void){

    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    //World

    auto world = simpler_random_scene();

    // Camera

    point3 lookfrom(13, 5, 13);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    double aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    //Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // --------------------------------------------------------------------
    //omp_set_dynamic(0);
    //omp_set_num_threads(THREAD_NUM);

    std::vector<std::vector<std::tuple<int, int, int>>> image(image_height, std::vector<std::tuple<int, int, int>>(image_width));

    //#pragma omp parallel for
    
    
    for(int j = image_height-1; j >= 0; --j){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j +  random_double()) / (image_height -1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            //#pragma omp critical
                //write_color(std::cout, pixel_color, samples_per_pixel);
            image = write_color_to_array(pixel_color, samples_per_pixel, image, i, j);
        }
    }
    


    //renderImage(image, image_height, image_width, world, samples_per_pixel, cam, max_depth);



    generate_image(image, image_height, image_width);
    
 

    std::cerr << "\nDone\n" << std::flush;

    

    // --------------------------------------------------------------------

    return 0;
}


void renderImage(std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width, hittable_list world, int samples_per_pixel, camera cam, int max_depth) {  
    #pragma omp parallel 
    {
    #pragma omp for
    for(int j = image_height-1; j >= 0; --j){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j +  random_double()) / (image_height -1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            //#pragma omp critical
                //write_color(std::cout, pixel_color, samples_per_pixel);
            //image = write_color_to_array(pixel_color, samples_per_pixel, image, i, j);

            // Do without the function call:

            std::tuple<int, int ,int> pixel;

            auto r = pixel_color.x();
            auto g = pixel_color.y();
            auto b = pixel_color.z();

            // Divide the color by the number fo samples and gamma correct for gamma=2.0
            auto scale = 1.0 / samples_per_pixel;
            r = sqrt(scale * r);
            g = sqrt(scale * g);
            b = sqrt(scale * b);

            pixel = std::make_tuple( static_cast<int>(256 * clamp(r, 0.0, 0.999)), static_cast<int>(256 * clamp(g, 0.0, 0.999)), static_cast<int>(256 * clamp(b, 0.0, 0.999)) );

            image[j][i] = pixel;

            // --------------------------------
        }
    }  
    } 
}


void generate_image(const std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width) {
    for(int j = image_height-1; j >= 0; --j){
        std::cerr << "\rWriting Lines (remaining): " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i){             
            std::cout << std::get<0>(image[j][i]) << ' ' << std::get<1>(image[j][i]) << ' ' << std::get<2>(image[j][i]) << '\n';
        }
    }
}



color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    if(depth <= 0) {
        return color(0,0,0);
    }

    if(world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}



hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5 ,0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));


    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if(choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if(choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


hittable_list simpler_random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5 ,0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

        //#pragma omp parallel for

    
        /*for(int a = -3; a < 3; a++) {
            for(int b = -3; b < 3; b++) {
                auto choose_mat = random_double();
                point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

                if((center - point3(4, 0.2, 0)).length() > 0.9) {
                    shared_ptr<material> sphere_material;

                    if(choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = make_shared<lambertian>(albedo);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    }
                    else if(choose_mat < 0.95) {
                        // metal
                        auto albedo = color::random(0.5, 1);
                        auto fuzz = random_double(0, 0.5);
                        sphere_material = make_shared<metal>(albedo, fuzz);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    }
                    else {
                        // glass
                        sphere_material = make_shared<dielectric>(1.5);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }*/
    


    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


