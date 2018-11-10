#ifndef __PHONG__
#define __PHONG__

#include "Material.h"
#include "Lambertian.h"
#include "GlossySpecular.h"

class Phong : public Material{
    public:
        // Variables
        Lambertian* ambient_brdf;           // Ambient BRDF
        Lambertian* diffuse_brdf;           // Diffuse BRDF
        GlossySpecular* specular_brdf;      // Specular BRDF
        
        // Big 6
        Phong(void);
        Phong(const double& k_a, const double& k_d, const double& k_s, const double& exp, const Vector3D& color);
        Phong(const Phong& phong);
        Phong& operator= (const Phong& rhs);
        ~Phong(void);
        virtual Material* clone(void) const;

        // Functions
        virtual Vector3D shade(ShadeRec& sr);
};

#endif
