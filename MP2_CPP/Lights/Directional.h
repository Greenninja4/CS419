#ifndef __DIRECTIONAL__
#define __DIRECTIONAL__

#include "Light.h"
#include "Vector3D.h"
#include "World.h"
#include "ShadeRec.h"

class Directional : public Light{
    public:
        // Variables
        float ls;           // Radiance
        Vector3D color;     // Color
        Vector3D dir;       // Direction

        // Big 6
        Directional(void);
        Directional(const double&  ls, const Vector3D& color, const Vector3D& dir);
        Directional(const Directional& directional);
        Directional& operator= (const Directional& rhs);
        virtual ~Directional(void);
        virtual Light* clone(void) const;

        // Functions
        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
        virtual bool casts_shadow(void) const;
        virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;
};

#endif