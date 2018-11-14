#ifndef __TRANSPARENT__
#define __TRANSPARENT__

#include "Phong.h"
#include "PerfectSpecular.h"
#include "PerfectTransmitter.h"

class Transparent : public Phong {
    public:
        // Variables
        PerfectSpecular* reflective_brdf;
        PerfectTransmitter* specular_btdf;

        // Big 6
        Transparent(void);
        Transparent(const double& k_a, const double& k_d, const double& k_s, const double& exp, const Vector3D& color, const double& k_r, const double& kt, const double& ior);
        Transparent(const Transparent& transparent);
        Transparent operator= (const Transparent& rhs);
        ~Transparent(void);
        virtual Material* clone(void) const;

        // Functions
        virtual Vector3D shade(ShadeRec& sr);
};

#endif
