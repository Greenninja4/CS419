#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "BRDF.h"

class Lambertian : public BRDF{
    public:
        float k_d;
        Vector3D color;

        Lambertian(void);
        Lambertian(float k_d, Vector3D color);
        Lambertian(const Lambertian& lambertian);
        virtual ~Lambertian(void);
        Lambertian& operator= (const Lambertian& rhs);
        virtual Lambertian* clone(void) const;

        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif