#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "GeometricObject.h"

class Triangle : public GeometricObject{
    public:
        // Variables
        Vector3D p1, p2, p3;    // Points
        Vector3D n;             // Normal
        BBox bbox;              // Bounding box

        // Big 6
        Triangle(void);
        Triangle(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3);
        Triangle(const Triangle& triangle);
        Triangle& operator= (const Triangle& rhs);
        virtual ~Triangle (void);
        virtual Triangle* clone(void) const;

        // Functions
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& s) const;
        // virtual BBox get_bounding_box(void);
        void compute_normal(void);
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

#endif