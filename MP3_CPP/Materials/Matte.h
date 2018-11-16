#ifndef __MATTE__
#define __MATTE__

#include "Material.h"
#include "Lambertian.h"

class Matte : public Material{
    public:
        Lambertian* ambient_brdf;
        Lambertian* diffuse_brdf;
        
        Matte(void);
        Matte(float k_a, float k_d, Vector3D color);
        Matte(const Matte& matte);
        Matte& operator= (const Matte& rhs);
        ~Matte(void);
        virtual Material* clone(void) const;

        virtual Vector3D shade(ShadeRec& sr);
        virtual Vector3D area_light_shade(ShadeRec& sr);
};

#endif
