#ifndef __EMMISSIVE__
#define __EMMISSIVE__

#include "Material.h"

class Emmissive : public Material{
    public:
        // Variables
        float ls;
        Vector3D ce;

        // Big 6
        Emmissive(void);
        Emmissive(float ls, const Vector3D& ce);
        Emmissive(const Emmissive& emmissive);
        Emmissive& operator= (const Emmissive& rhs);
        ~Emmissive(void);
        Material* clone(void) const;

        // Variables
        virtual Vector3D shade(ShadeRec& sr);
        virtual Vector3D area_light_shade(ShadeRec& sr);
        virtual Vector3D get_Le(ShadeRec& sr) const;
};

#endif