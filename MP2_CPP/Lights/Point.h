#ifndef __POINT__
#define __POINT__

#include "Light.h"

class Point : public Light{
    public:
        // Variables
        float ls;           // Radiance (magnitude)
        Vector3D color;     // Color of light
        Vector3D location;  // Location of light

        // Big 6
        Point(void);
        Point(const double& ls, const Vector3D& color, const Vector3D& location);
        Point(const Point& point);
        virtual Light* clone(void) const;
        Point& operator= (const Point& rhs);
        virtual ~Point(void);
        
        // Functions
        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
        virtual bool casts_shadow(void) const;
        virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;
};

#endif