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
            // From https://github.com/RayTracing/raytracing.github.io/blob/release/src/InOneWeekend/sphere.h
            vec3 oc = center - r.orig();
            auto a = r.dir().length_squared();
            auto h = dot(r.dir(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0)
                return false;

            auto sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }
    private:
        vec3 center;
        double radius;
        material mat;
};

#endif