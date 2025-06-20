#ifndef INTERVAL_H
#define INTERVAL_H

#include "useful_functions.h"
class interval 
{
    public:
        double min, max;

        interval() : min(+infinity), max(-infinity) {}

        interval(double a, double b) : min(a), max(b) {}

        double size() const 
        {
            return max - min;
        }

        bool constains(double x) const 
        {
            return x >= min && x <= max;
        }

        bool surrounds(double x) const 
        {
            return x > min && x < max;
        }

        static const interval empty, universe;
    private:
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif