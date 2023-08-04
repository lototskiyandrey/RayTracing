

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


hittable_list final_scene();

int main(void){

    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    //World

    auto world = final_scene();
    color background(0.0,0.0,0.0);

    // Camera

    point3 lookfrom(0, 0, 22);
    point3 lookat(0, 0.5, 1);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    double aperture = 0.01;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);

    //Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // --------------------------------------------------------------------
    //omp_set_dynamic(0);
    //omp_set_num_threads(THREAD_NUM);

    std::vector<std::vector<std::tuple<int, int, int>>> image(image_height, std::vector<std::tuple<int, int, int>>(image_width));
    
    omp_set_num_threads(4);

    renderImage(image, image_height, image_width, world, samples_per_pixel, cam, max_depth, background);



    generate_image(image, image_height, image_width);
    
 

    std::cerr << "\nDone\n" << std::flush;

    

    // --------------------------------------------------------------------

    return 0;
}


void renderImage(std::vector<std::vector<std::tuple<int, int, int>>> &image, const int image_height, const int image_width, hittable_list world, int samples_per_pixel, camera cam, int max_depth, color background) {  
    //omp_set_num_threads(4);
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



hittable_list final_scene() {
    hittable_list world;


    auto ground_material = make_shared<lambertian>(color(0.5, 0.5 ,0.5));
    auto diff_light_blue = make_shared<diffuse_light>(color(46.0/40, 147.0/40, 255.0/40));
    auto diff_light_red = make_shared<diffuse_light>(color(255.0/40, 81.0/40, 46.0/40));
    

    //world.add(make_shared<rect_xy>(0, 0, 10, 5, 2, diff_light));
    world.add(make_shared<rect_xz>(0, 2, 0.5, 3, -2.8, diff_light_blue));
    world.add(make_shared<rect_xz>(-2, 0, 0.5, 3, -2.8, diff_light_red));
    //world.add(make_shared<rect_xz>(0, 555, 0, 555, 555, ground_material));

    auto cyan = make_shared<lambertian>(color(52.0/255, 235.0/255, 213.0/255));
    world.add(make_shared<sphere>(point3(4, 2.505,3), 0.5, cyan));

    auto glass = make_shared<dielectric>(1.3);
    world.add(make_shared<sphere>(point3(-1.2, 1.8, 0.7), 0.7, glass));

    auto orange = make_shared<metal>(color(245.0/255, 148.0/255, 2.0/255), 0.1);
    world.add(make_shared<sphere>(point3(-3.4, 1.8, 4), 1, orange));

    auto purple = make_shared<lambertian>(color(146/255.0, 45.0/255.0, 247.0/255.0));
    world.add(make_shared<rect_xz>(-2, 2, -2, 4, 2.8, purple));

    auto light_bulb = make_shared<diffuse_light>(color(250.0/255, 252.0/255, 202.0/255));
    world.add(make_shared<sphere>(point3(2.0, 1.5, 0.7), 0.3, light_bulb));

    auto green = make_shared<lambertian>(color(26.0/255, 176.0/255, 41.0/255));
    world.add(make_shared<sphere>(point3(-2.3, -0.3, 1.7), 1.2, green));
    
    auto pink = make_shared<metal>(color(189.0/255, 0, 151.0/255), 1);
    world.add(make_shared<sphere>(point3(2.4, 2.6,5.3), 0.3, pink));

    auto blue = make_shared<metal>(color(12.0/255, 36.0/255, 196.0/255), 0.5);
    world.add(make_shared<rect_xy>(-2, 2, -3, 1.5, -1.5, blue));


    world.add(make_shared<rect_xz>(-5, 5, -2, 7, 3.05, ground_material));


    return world;    
}














