#ifndef __POINT__
#define __POINT__

#include "Light.h"

class Point : public Light{
    public:
        // Radiance, color, location
        float ls;
        Vector3D color;
        Vector3D location;

        Point(void);
        Point(float ls, Vector3D color, Vector3D location);
        Point(const Point& point);
        virtual Light* clone(void) const;
        Point& operator= (const Point& rhs);
        virtual ~Point(void);
        
        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
};

#endif