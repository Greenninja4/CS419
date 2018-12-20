#ifndef __SV_LAMBERTIAN__
#define __SV_LAMBERTIAN__

#include "BRDF.h"
#include "Texture.h"

// Spatially Varying Lambertian (used for textures)
class SV_Lambertian : public BRDF{
    public:
        float k_d;
        Texture* texture_ptr;

        SV_Lambertian(void);
        SV_Lambertian(float k_d, Texture* texture);
        SV_Lambertian(const SV_Lambertian& sv_lambertian);
        virtual ~SV_Lambertian(void);
        SV_Lambertian& operator= (const SV_Lambertian& rhs);
        virtual SV_Lambertian* clone(void) const;

        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif