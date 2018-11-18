#ifndef __MATERIAL__
#define __MATERIAL__

#include "World.h"
#include "Vector3D.h"
#include "ShadeRec.h"

class Material{
    public:
        // Big 6
        Material(void);
        Material(const Material& material);
        Material& operator= (const Material& rhs);
        virtual ~Material(void);
        virtual Material* clone(void) const = 0;

        // Functions
        virtual Vector3D shade(ShadeRec& sr);
        virtual Vector3D area_light_shade(ShadeRec& sr);
        virtual Vector3D get_Le(ShadeRec& sr) const;
};

#endif