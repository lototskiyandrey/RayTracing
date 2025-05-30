#include <iostream>

int main() 
{   

    int image_width = 500;
    int image_height = 500;

    std::cout << "P3\n " << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {
            auto r = (double)i / image_width;
            auto g = (double)j / image_height;
            auto b = 0;

            auto ir = int(r * 255);
            auto ig = int(g * 255);
            auto ib = int(b * 255);

            std::cout << ir << ' ' << ig << ' ' << ib << std::endl;
        }
    }

    std::clog << "\rDone.                             \n";
    

    return 0;
}