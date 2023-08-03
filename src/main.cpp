#include "./headers/rtweekend.h"


#include "./headers/color.h"
#include "./headers/ray.h"
#include "./headers/hittable_list.h"
#include "./headers/vec3.h"
#include "./headers/sphere.h"
#include "./headers/camera.h"
#include "./headers/material.h"
#include "./headers/aarect.h"

//#include <corecrt_math.h>
#include <iostream>
#include <memory>


color ray_color(const ray &r, const color &background, const hittable &world, int depth);

hittable_list random_scene();

hittable_list texture_scene();

int main(void){

    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
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
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n" << std::flush;

    return 0;
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


