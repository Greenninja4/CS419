#ifndef __AMBIENT__
#define __AMBIENT__

#include "Light.h"

class Ambient : public Light{
    public:
        // Radiance, color
        float ls;
        Vector3D color;

        Ambient(void);
        Ambient(float ls, Vector3D color);
        Ambient(const Ambient& ambient);
        Ambient& operator=(const Ambient& rhs);
        virtual ~Ambient(void);
        virtual Light* clone(void) const;

        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
};

#endif