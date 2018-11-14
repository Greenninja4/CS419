#ifndef __BTDF__
#define __BTDF__

#include "Vector3D.h"
#include "ShadeRec.h"

class BTDF {
    public:
        BTDF(void);
        BTDF(const BTDF& btdf);
        BTDF& operator= (const BTDF& rhs);
        virtual ~BTDF(void);
        virtual BTDF* clone(void) const = 0;
        
        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const = 0;
        virtual Vector3D sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const = 0;
        virtual Vector3D rho(const ShadeRec& sr, const Vector3D& wo) const = 0;
        virtual bool tir(const ShadeRec& sr) const = 0;
};

#endif