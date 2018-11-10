#ifndef __RAY__
#define __RAY__

#include "Vector3D.h"

class Ray{
    public:
        // Variables
        Vector3D o; // Origin
        Vector3D d; // Direction

        // Big 6
        Ray(void);
        Ray(const Vector3D& o, const Vector3D& d);
        Ray(const Ray& ray);
        Ray& operator= (const Ray& rhs);
        ~Ray();

        // Getters & Setters
        void set_o(const Vector3D& o);
        void set_d(const Vector3D& d);
        Vector3D get_o(void) const;
        Vector3D get_d(void) const;
};

#endif