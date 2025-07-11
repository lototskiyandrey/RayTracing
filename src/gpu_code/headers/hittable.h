#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "material.h"

class hit_record 
{
    public:
        vec3 p;
        vec3 normal;
        double t;
        bool front_face;
        material mat;
        void set_face_normal(const ray &r, const vec3 &outward_normal)
        {
            // Set the hit record normal vector to point outwards

            front_face = dot(r.dir(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable 
{
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

class sphere : public hittable 
{
    public:
        sphere(vec3 center, double radius, material mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

        bool hit(const ray &r, interval ray_t, hit_record &rec) const override 
        {
            auto descriminant = hit_sphere(r);
            if(descriminant < 0)
            {
                return false;
            }

            double a = dot(r.dir(), r.dir());
            double b = dot(-2*r.dir(), center - r.orig());

            auto positive_root = (-b + std::sqrt(descriminant)) / (2 * a);
            if(!ray_t.surrounds(positive_root))
            {
                auto negative_root = (-b - std::sqrt(descriminant)) / (2 * a);
                if(!ray_t.surrounds(negative_root))
                {
                    return false;
                }
                rec.t = negative_root;
                rec.p = r.at(negative_root);
                rec.normal = unit_vector(r.at(negative_root) - center);
                vec3 outward_normal = (rec.p - center) / radius;
                rec.set_face_normal(r, outward_normal);
                return true;
            }
            rec.t = positive_root;
            rec.p = r.at(positive_root);
            rec.normal = unit_vector(r.at(positive_root) - center);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;
            return true;
        }
    private:
        vec3 center;
        double radius;
        material mat;

        double hit_sphere(ray r) const
        {
            double a = dot(r.dir(), r.dir());
            double b = dot(-2*r.dir(), center - r.orig());
            double c = dot(center - r.orig(), center - r.orig()) - (radius * radius);
            double discriminant = b*b - 4*a*c;

            if(discriminant < 0)
            {
                return -1.0;
            }

            return discriminant;
        }
};

#endif