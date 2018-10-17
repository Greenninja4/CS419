#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "GeometricObject.h"

class Triangle: public GeometricObject{
    public:
        // Point 1, 2, and 3, normal
        Vector3D p1;
        Vector3D p2;
        Vector3D p3;
        Vector3D n;

        Triangle(void);
        Triangle(Vector3D p1, Vector3D p2, Vector3D p3, const Vector3D color);
        Triangle(const Triangle& triangle);
        Triangle& operator=(const Triangle& rhs);
        ~Triangle (void);

        // Member functions
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
};

#endif