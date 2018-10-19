#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"

class Sphere : public GeometricObject{
    public:
        // Variables
        Vector3D center;    // Center
        double radius;      // Radius

        // Big 6
        Sphere(void);
        Sphere(const Vector3D& center, const double& radius);
        Sphere(const Sphere& sphere);
        Sphere& operator= (const Sphere& rhs);
        virtual ~Sphere (void);
        virtual Sphere* clone(void) const;

        // Getters & Setters
        void set_center(const Vector3D& center);
        void set_radius(const double& r);

        // Functions
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
};

#endif