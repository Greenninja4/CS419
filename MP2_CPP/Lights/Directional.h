#ifndef __DIRECTIONAL__
#define __DIRECTIONAL__

#include "Light.h"
#include "Vector3D.h"
#include "World.h"
#include "ShadeRec.h"

class Directional : public Light{
    public:
        // Radiance, color, direction
        float ls;
        Vector3D color;
        Vector3D dir;

        Directional(void);
        Directional(float ls, Vector3D color, Vector3D dir);
        Directional(const Directional& directional);
        Directional& operator=(const Directional& rhs);
        virtual ~Directional(void);
        virtual Light* clone(void) const;

        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
};

#endif