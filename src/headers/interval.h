#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>

class interval 
{
    public:
        double min, max;
        interval() : min(+infinity), max(-infinity) {}

        interval(double min, double max) : min(min), max(max) {}

        double size() const 
        {
            return max - min;
        }

        bool contains(double x) const 
        {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const 
        {
            return min < x && x < max;
        }

        double clamp(double x) const 
        {
            if(x < min) 
            {
                return min;
            }
            if(x > max) 
            {
                return max;
            }
            return x;
        }

        interval create_empty()
        {
            return interval(+infinity, -infinity);
        }

        interval create_universe()
        {
            return interval(-infinity, +infinity);
        }

        static const interval empty, universe;

    private:
        // Useful Constants
        const double infinity = std::numeric_limits<double>::infinity();
        const double pi = 3.1415926535897932385;
        
};

const interval interval::empty = interval().create_empty();
const interval interval::universe = interval().create_universe();

#endif