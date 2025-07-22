#ifndef MATERIAL_H
#define MATERIAL_H

#include "useful_functions.h"
#include "vec3.h"

class material 
{
    public:
        double albedo; // the probability of light to reflect, or be absorbed
        std::vector<double> color; // the color of the object
        // double emittivity; // How much light the material emits.
        bool is_refractive;

        material() {}

        material(double albedo, std::vector<double> colors, bool is_refractive) : albedo(albedo), is_refractive(is_refractive)
        {
            for(int i = 0; i < hue_length; i++) 
            {
                color.push_back(colors[i]);
            }
        }

    private:
};

#endif