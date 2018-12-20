#ifndef __SV_MATTE__
#define __SV_MATTE__

#include "Material.h"
#include "SV_Lambertian.h"
#include "Texture.h"

class SV_Matte : public Material{
    public:
        SV_Lambertian* ambient_brdf;
        SV_Lambertian* diffuse_brdf;
        
        SV_Matte(void);
        SV_Matte(float k_a, float k_d, Texture* texture_ptr);
        SV_Matte(const SV_Matte& sv_matte);
        SV_Matte& operator= (const SV_Matte& rhs);
        ~SV_Matte(void);
        virtual Material* clone(void) const;

        virtual Vector3D shade(ShadeRec& sr);
        virtual Vector3D area_light_shade(ShadeRec& sr);
};

#endif
