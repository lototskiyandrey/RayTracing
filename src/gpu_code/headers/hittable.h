#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"

class hit_record 
{
    public:
        vec3 p;
        vec3 normal;
        double t;
};

class hittable 
{
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const = 0;
};

class sphere : public hittable 
{
    public:
        sphere(vec3 center, double radius) : center(center), radius(std::fmax(0, radius)) {}

        bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override 
        {
            auto descriminant = hit_sphere(r);
            if(descriminant < 0)
            {
                return false;
            }

            double a = dot(r.dir(), r.dir());
            double b = dot(-2*r.dir(), center - r.orig());

            auto positive_root = (-b + std::sqrt(descriminant)) / (2 * a);
            if(positive_root < ray_tmin || positive_root > ray_tmax)
            {
                auto negative_root = (-b - std::sqrt(descriminant)) / (2 * a);
                if(negative_root < ray_tmin || negative_root > ray_tmax)
                {
                    return false;
                }
                rec.t = negative_root;
                rec.p = r.at(negative_root);
                rec.normal = unit_vector(r.at(negative_root) - center);
            }
            rec.t = positive_root;
            rec.p = r.at(positive_root);
            rec.normal = unit_vector(r.at(positive_root) - center);
            
            return true;
        }
    private:
        vec3 center;
        double radius;

        double hit_sphere(ray r) const
        {
            double a = dot(r.dir(), r.dir());
            double b = dot(-2*r.dir(), center - r.orig());
            double c = dot(center - r.orig(), center - r.orig()) - (radius * radius);
            double descriminant = b*b - 4*a*c;

            if(descriminant < 0)
            {
                return -1.0;
            }

            return descriminant;
        }
};

#endif