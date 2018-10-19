#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "Ray.h"

class ShadeRec;

class Light{
    public:
        Light(void);
        Light(const Light& light);
        Light& operator=(const Light& rhs);
        virtual Light* clone(void) const = 0;
        virtual ~Light(void);

        virtual Vector3D get_direction(ShadeRec& sr) = 0;
        virtual Vector3D L(ShadeRec& sr);
};

#endif