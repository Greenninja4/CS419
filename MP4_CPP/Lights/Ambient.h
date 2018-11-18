#ifndef __AMBIENT__
#define __AMBIENT__

#include "Light.h"

class Ambient : public Light{
    public:
        // Variables
        float ls;           // Radiance (magnitude)
        Vector3D color;     // Color of light

        // Big 6
        Ambient(void);
        Ambient(const double& ls, const Vector3D& color);
        Ambient(const Ambient& ambient);
        Ambient& operator= (const Ambient& rhs);
        virtual ~Ambient(void);
        virtual Light* clone(void) const;

        // Functions
        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
};

#endif