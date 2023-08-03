

#include "./headers/rtweekend.h"
#include "./headers/color.h"
#include "./headers/ray.h"
#include "./headers/hittable_list.h"
#include "./headers/vec3.h"
#include "./headers/sphere.h"
#include "./headers/camera.h"
#include "./headers/material.h"
#include "./headers/aarect.h"

#include <iostream>
#include <memory>
#include <omp.h>






void generate_image(const std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width);
void renderImage(std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width, hittable_list world, int samples_per_pixel, camera cam, int max_depth, color background);


color ray_color(const ray &r, const color &background, const hittable &world, int depth);


hittable_list texture_scene();

int main(void){

    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //World

    auto world = texture_scene();
    color background(0.00,0.00,0.00);

    // Camera

    point3 lookfrom(0, 0.7, 20);
    point3 lookat(0, 0, 1);
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
    
    renderImage(image, image_height, image_width, world, samples_per_pixel, cam, max_depth, background);



    generate_image(image, image_height, image_width);
    
 

    std::cerr << "\nDone\n" << std::flush;

    

    // --------------------------------------------------------------------

    return 0;
}


void renderImage(std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width, hittable_list world, int samples_per_pixel, camera cam, int max_depth, color background) {  
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
                pixel_color += ray_color(r, background, world, max_depth);
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


color ray_color(const ray &r, const color &background, const hittable &world, int depth) {
    hit_record rec;

    if(depth <= 0) {
        return color(0,0,0);
    }

    if(!world.hit(r, 0.001, INFINITY, rec)) {
        return background;
    }

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if(!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}



hittable_list texture_scene() {
    hittable_list world;


    auto ground_material = make_shared<lambertian>(color(0.5, 0.5 ,0.5));
    auto diff_light = make_shared<diffuse_light>(color(3.0, 3.0, 3.0));
    

    //world.add(make_shared<rect_xy>(0, 0, 10, 5, 2, diff_light));
    world.add(make_shared<rect_xz>(-3, 3, -0.5, 4, -2.5, diff_light));
    //world.add(make_shared<rect_xz>(0, 555, 0, 555, 555, ground_material));

    auto blue = make_shared<lambertian>(color(24.0/255, 153.0/255, 240.0/255));

    world.add(make_shared<rect_xz>(-3, 3, 1, 4, 2.7, blue));

    return world;    
}














