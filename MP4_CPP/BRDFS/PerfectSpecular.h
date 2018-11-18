#ifndef __PERFECT_SPECULAR__
#define __PERFECT_SPECULAR__

#include "BRDF.h"

class PerfectSpecular : public BRDF {
    public:
        float k_r;
        Vector3D color;

        PerfectSpecular(void);
        PerfectSpecular(float k_r, Vector3D color);
        PerfectSpecular(const PerfectSpecular& specular);
        virtual ~PerfectSpecular(void);
        PerfectSpecular& operator= (const PerfectSpecular& rhs);
        virtual PerfectSpecular* clone(void) const;

        virtual Vector3D sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif