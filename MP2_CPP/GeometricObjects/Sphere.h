#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"

class Sphere: public GeometricObject{
    public:
        // Center, radius
        Vector3D center;
        double radius;

        Sphere(void);
        Sphere(Vector3D center, double radius);
        Sphere(const Sphere& sphere);
        Sphere& operator= (const Sphere& rhs);
        virtual ~Sphere (void);
        virtual Sphere* clone(void) const;

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
};

#endif