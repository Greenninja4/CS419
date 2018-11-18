#ifndef __BRDF__
#define __BRDF__

#include "Vector3D.h"
#include "ShadeRec.h"

class BRDF{
    public:
        BRDF(void);
        BRDF(const BRDF& brdf);
        BRDF& operator= (const BRDF& rhs);
        virtual ~BRDF(void);
        virtual BRDF* clone(void) const = 0;
        
        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        virtual Vector3D sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif