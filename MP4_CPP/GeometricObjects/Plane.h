#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

class Plane : public GeometricObject{
    public:
        // Variables
        Vector3D p; // Point
        Vector3D n; // Normal
        BBox bbox;  // BBox

        // Big 6
        Plane(void);
        Plane(const Vector3D& p, const Vector3D& n);
        Plane(const Plane& plane);
        Plane& operator= (const Plane& rhs);
        ~Plane (void);
        virtual Plane* clone(void) const;

        // Function
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& s) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

#endif