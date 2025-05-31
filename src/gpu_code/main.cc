#include <bits/stdc++.h>
#include "headers/useful_functions.h"
#include "headers/vec3.h"

void write_color(std::ostream &out, std::vector<double> spectrum);

int main() 
{   
    // Image
    int image_width = 700;
    int image_height = 500;

    // Camera


    std::cout << "P3\n " << image_width << " " << image_height << "\n255\n";

    std::vector<double> spectrum(spectrum_length);
    load_and_populate_cmfs(cmf_r, cmf_g, cmf_b);
    for(int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {
            generate_spectrum_gaussian(spectrum, j/1.5+380, 100);
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



