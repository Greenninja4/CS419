#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "Ray.h"

class ShadeRec;     // Forward declaration because idk?

class Light{
    public:
        // Variables
        // bool shadows;   // Shadows are in effect for light?

        // Big 6
        Light(void);
        Light(const Light& light);
        Light& operator= (const Light& rhs);
        virtual Light* clone(void) const = 0;
        virtual ~Light(void);

        // Functinos
        virtual Vector3D get_direction(ShadeRec& sr) = 0;
        virtual Vector3D L(ShadeRec& sr);
        virtual bool casts_shadow(void) const;
        virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;
        virtual float G(const ShadeRec& sr) const;
        virtual float pdf(const ShadeRec& sr) const;
};

#endif