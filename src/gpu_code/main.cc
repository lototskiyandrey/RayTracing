#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include "headers/useful_functions.h"

void load_and_populate_cmfs(std::vector<double> &cmf_r, std::vector<double> &cmf_g, std::vector<double> &cmf_b);
void generate_spectrum_gaussian(std::vector<double> &spectrum, int mean, int variance);
void write_color(std::ostream &out, std::vector<double> spectrum, std::vector<double> cmf_r, std::vector<double> cmf_g, std::vector<double> cmf_b);

int longest_wavelength = 830;
int shortest_wavelength = 390;

double clamp(double a, double b, double c);
double gaussian(int mean, int variance, int at);

int main() 
{   

    int spectrum_length = longest_wavelength - shortest_wavelength + 1;

    std::vector<double> spectrum(spectrum_length);

    std::vector<double> cmf_r(spectrum_length);
    std::vector<double> cmf_g(spectrum_length);
    std::vector<double> cmf_b(spectrum_length);



    load_and_populate_cmfs(cmf_r, cmf_g, cmf_b);

    // generate_spectrum_gaussian(spectrum, 510, 30);


    int image_width = 500;
    int image_height = 500;

    std::cout << "P3\n " << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++)
        {
            generate_spectrum_gaussian(spectrum, j/1.5+380, 50);
            write_color(std::cout, spectrum, cmf_r, cmf_g, cmf_b);
        }
    }

    std::clog << "\rDone.                             \n";


    

    return 0;
}

/*

Implmenting a more realistic color system. Every ray is now an array where each element corresponds to the intesity of
a particular wavelength of visible light

*/

void load_and_populate_cmfs(std::vector<double> &cmf_r, std::vector<double> &cmf_g, std::vector<double> &cmf_b) 
{
    std::ifstream f("cmf.csv");

    if(!f.is_open())
    {
        std::cerr << "Error opening file!";
        exit(-1);
    }

    std::string s;


    while(std::getline(f, s))
    {   
        int row_num = 0;
        std::string elems[4];

        std::string delimiter = ",";

        auto pos = s.find(delimiter);

        while(pos != std::string::npos)
        {
            elems[row_num] = s.substr(0, pos);

            s.erase(0, pos + delimiter.length());

            pos = s.find(delimiter);

            row_num++;
        }

        elems[row_num] = s;


        int wavelength = std::stoi(elems[0]);
        int index = wavelength - shortest_wavelength;

        double red_val = std::stod(elems[1]);
        double green_val = std::stod(elems[2]);
        double blue_val = std::stod(elems[3]);
        
        cmf_r.at(index) = red_val;
        cmf_g.at(index) = green_val;
        cmf_b.at(index) = blue_val;
    }

    f.close();
}


double integrate_spectrum_cmf(std::vector<double> spectrum, std::vector<double> cmf)
{
    double tri_stim = 0.0;

    for(int i = 0; i < spectrum.size(); i++) 
    {
        tri_stim += spectrum.at(i) * cmf.at(i);
    }

    return tri_stim;
}


void write_color(std::ostream &out, std::vector<double> spectrum, std::vector<double> cmf_r, std::vector<double> cmf_g, std::vector<double> cmf_b)
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

double clamp(double a, double b, double c) 
{
    if(a < b)
    {
        return b;
    }
    if(a > c)
    {
        return c;
    }
    return a;
}

void generate_spectrum_gaussian(std::vector<double> &spectrum, int mean, int variance)
{
    for(int i = 0; i < spectrum.size(); i++)
    {
        spectrum.at(i) = gaussian(mean, variance, i+shortest_wavelength);
    }
}

double gaussian(int mean, int variance, int at)
{
    return (1 / std::sqrt(2 * pi * variance * variance)) * std::pow(e, ((-(at-mean) * (at-mean)) / (2 * variance * variance)));
}