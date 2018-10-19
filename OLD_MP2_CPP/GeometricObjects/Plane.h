#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

class Plane : public GeometricObject{
    public:
        // Point, normal
        Vector3D p;
        Vector3D n;

        Plane(void);
        Plane(const Vector3D& p, const Vector3D& n);
        Plane(const Plane& plane);
        Plane& operator= (const Plane& rhs);
        ~Plane (void);
        virtual Plane* clone(void) const;

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
};

#endif