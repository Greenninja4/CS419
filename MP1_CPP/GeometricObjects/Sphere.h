#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"

class Sphere: public GeometricObject{
    public:
        // Center, radius
        Vector3D center;
        double radius;

        Sphere(void);
        Sphere(Vector3D center, double radius, const Vector3D color);
        Sphere(const Sphere& sphere);
        Sphere& operator= (const Sphere& rhs);
        ~Sphere (void);

        // Member functions
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
};

#endif