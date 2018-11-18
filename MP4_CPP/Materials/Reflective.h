#ifndef __REFLECTIVE__
#define __REFLECTIVE__

#include "Phong.h"
#include "PerfectSpecular.h"

class Reflective : public Phong {
    public:
        // Variables
        PerfectSpecular* reflective_brdf;

        // Big 6
        Reflective(void);
        Reflective(const double& k_a, const double& k_d, const double& k_s, const double& exp, const Vector3D& color, const double& k_r);
        Reflective(const Reflective& reflective);
        Reflective operator= (const Reflective& rhs);
        ~Reflective(void);
        virtual Material* clone(void) const;

        // Functions
        virtual Vector3D shade(ShadeRec& sr);
};

#endif
