#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"

#include "hittable.h"

// ---------------------------------------------------------------------------------

class rect_xy : public hittable {

    public:
        rect_xy() {}

        rect_xy(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

        

    public:
        shared_ptr<material> mp;
        double x0, y0, x1, y1, k;    

};


bool rect_xy::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();

    if(t < t_min || t > t_max) {
        return false;
    } 

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();   

    if(x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

// ---------------------------------------------------------------------------------

class rect_xz : public hittable {

    public:
        rect_xz() {}

        rect_xz(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

        

    public:
        shared_ptr<material> mp;
        double x0, z0, x1, z1, k;    

};


bool rect_xz::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().y()) / r.direction().y();

    if(t < t_min || t > t_max) {
        return false;
    } 

    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();   

    if(x < x0 || x > x1 || z < z0 || z > z1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

// ---------------------------------------------------------------------------------

class rect_yz : public hittable {

    public:
        rect_yz() {}

        rect_yz(double _z0, double _z1, double _y0, double _y1, double _k, shared_ptr<material> mat) : z0(_z0), z1(_z1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

        

    public:
        shared_ptr<material> mp;
        double y0, z0, y1, z1, k;    

};


bool rect_yz::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().x()) / r.direction().x();

    if(t < t_min || t > t_max) {
        return false;
    } 

    auto z = r.origin().z() + t * r.direction().z();
    auto y = r.origin().y() + t * r.direction().y();   

    if(z < z0 || z > z1 || y < y0 || y > y1) {
        return false;
    }

    rec.v = (z - z0) / (z1 - z0);
    rec.u = (y - y0) / (y1 - y0);
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

// ---------------------------------------------------------------------------------

#endif