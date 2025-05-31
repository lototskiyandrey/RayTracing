#ifndef RAY_H
#define RAY_H
#include "vec3.h"
class ray 
{
    public:
        ray() {}
        ray(const vec3 orig, const vec3 dir) : origin(orig), direction(dir) {}

        const vec3 orig() const {return origin;}
        const vec3 dir() const {return direction;}

        vec3 at(double t)
        {
            return origin + direction * t;
        }

    private:
        vec3 origin;
        vec3 direction;
};

#endif