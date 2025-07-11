#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <fstream>
// #include <bits/stdc++.h>
#include <algorithm>
#include <functional>

// Namespaces
using std::make_shared;
using std::shared_ptr;

// Useful Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double e = 2.7182818284590452353;

const int longest_wavelength = 830;
const int shortest_wavelength = 390;
const int spectrum_length = longest_wavelength - shortest_wavelength + 1;

std::vector<double> cmf_r(spectrum_length);
std::vector<double> cmf_g(spectrum_length);
std::vector<double> cmf_b(spectrum_length);

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

// Return a random real in [0,1)
inline double random_double() 
{
    return std::rand() / (RAND_MAX + 1.0);
}

// Return a random real in [min, max)
inline double random_double(double min, double max) 
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return int(random_double(min, max+1));
}

inline double clamp(double a, double b, double c) 
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

inline double gaussian(int mean, int variance, int at)
{
    return (1 / std::sqrt(2 * pi * variance * variance)) * std::pow(e, ((-(at-mean) * (at-mean)) / (2 * variance * variance)));
}

inline void load_and_populate_cmfs(std::vector<double> &cmf_r, std::vector<double> &cmf_g, std::vector<double> &cmf_b) 
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

inline double integrate_spectrum_cmf(std::vector<double> spectrum, std::vector<double> cmf)
{
    double tri_stim = 0.0;

    for(int i = 0; i < spectrum.size(); i++) 
    {
        tri_stim += spectrum.at(i) * cmf.at(i);
    }

    return tri_stim;
}

inline void generate_spectrum_gaussian(std::vector<double> &spectrum, int mean, int variance)
{
    for(int i = 0; i < spectrum_length; i++)
    {
        spectrum.at(i) = gaussian(mean, variance, i+shortest_wavelength);
    }
}

inline std::vector<double> multiply_vector_by_constant(std::vector<double> vec1, double factor)
{   

    std::vector<double> vec_result(spectrum_length);

    for(int i = 0; i < vec1.size(); i++)
    {
        vec_result[i] = vec1[i] * factor;
    }

    return vec_result;
}


inline void generate_spectrum_uniform(std::vector<double> &spectrum, int begin, int end)
{
    for(int i = begin; i < end; i++)
    {
        spectrum[i] = 1.0 / (end - begin);
    }
}

inline void superposition_spectrum(std::vector<double> &spectrum, std::vector<double> f, std::vector<double> s)
{
    for(int i = 0; i < spectrum.size(); i++)
    {
        spectrum[i] = f[i] + s[i];
    }
}

inline std::vector<double> multiply_two_vectors(std::vector<double> vec1, std::vector<double> vec2)
{
    std::vector<double> vec_result(spectrum_length);

    for(int i = 0; i < vec_result.size(); i++)
    {
        vec_result[i] = vec1[i] * vec2[i];
    }

    return vec_result;
}

inline std::vector<double> create_zero_vector(int length)
{
    std::vector<double> vec(length);

    for(int i = 0; i < vec.size(); i++)
    {
        vec[i] = 0.0;
    }

    return vec;
}

inline bool is_zero_vector(std::vector<double> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i) != 0.0)
        {
            return false;
        }
    }

    return true;
}

#endif