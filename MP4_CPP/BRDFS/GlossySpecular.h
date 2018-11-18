#ifndef __GLOSSY_SPECULAR__
#define __GLOSSY_SPECULAR__

#include "BRDF.h"

class GlossySpecular : public BRDF{
    public:
        // Variables
        float k_s;          // Specular coefficient
        Vector3D color;     // Specular color
        float exp;          // Specular exponent
        // Sampler* sampler;   // Sampler

        // Big 6
        GlossySpecular(void);
        GlossySpecular(const double& k_s, const Vector3D& color, const double& exp);
        GlossySpecular(const GlossySpecular& glossy_specular);
        virtual ~GlossySpecular(void);
        GlossySpecular& operator= (const GlossySpecular& rhs);
        virtual GlossySpecular* clone(void) const;

        // Functions
        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // virtual Vector3D sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, double pdf) const;
        virtual Vector3D rho(const ShadeRec& sr, Vector3D& wo) const;
};

#endif