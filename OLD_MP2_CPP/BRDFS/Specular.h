#ifndef __SPECULAR__
#define __SPECULAR__

#include "BRDF.h"

class Specular : public BRDF{
    public:
        float k_r;
        Vector3D color;

        Specular(void);
        Specular(float k_r, Vector3D color);
        Specular(const Specular& specular);
        virtual ~Specular(void);
        Specular& operator=(const Specular& rhs);
        virtual Specular* clone(void) const;

        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif