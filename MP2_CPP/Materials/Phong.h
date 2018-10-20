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
        Phong(const float& k_a, const float& k_d, const float& k_s, const float& exp, const Vector3D& color);
        Phong(const Phong& phong);
        Phong& operator= (const Phong& rhs);
        ~Phong(void);
        virtual Material* clone(void) const;

        // Functions
        virtual Vector3D shade(ShadeRec& sr);
};

#endif
