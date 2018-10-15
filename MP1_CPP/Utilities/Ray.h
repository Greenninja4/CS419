#ifndef __RAY__
#define __RAY__

#include "Vector3D.h"

class Ray{
    public:
        // Origin, direction
        Vector3D o;
        Vector3D d;

        Ray(void);
        Ray(const Vector3D& o, const Vector3D& d);
        Ray(const Ray& ray);
        Ray& operator=(const Ray& rhs);
        ~Ray();
};

#endif