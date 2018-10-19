#ifndef __PHONG__
#define __PHONG__

#include "Material.h"
#include "Lambertian.h"

class Phong : public Material{
    public:
        Lambertian* ambient_brdf;
        Lambertian* diffuse_brdf;
        
        Phong(void);
        Phong(float k_a, float k_d, Vector3D color);
        Phong(const Phong& phong);
        Phong& operator=(const Phong& rhs);
        ~Phong(void);
        virtual Material* clone(void) const;

        virtual Vector3D shade(ShadeRec& sr);
};

#endif
