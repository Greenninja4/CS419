#ifndef __SMOOTH_TRIANGLE__
#define __SMOOTH_TRIANGLE__

#include "GeometricObject.h"

class SmoothTriangle : public GeometricObject{
    public:
        // Variables
        Vector3D p1, p2, p3;    // Points
        Vector3D n, n1, n2, n3; // Vertex normals
        BBox bbox;              // Bounding box

        // Big 6
        SmoothTriangle(void);
        SmoothTriangle(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& n1, const Vector3D& n2, const Vector3D& n3);
        SmoothTriangle(const SmoothTriangle& tri);
        SmoothTriangle& operator= (const SmoothTriangle& rhs);
        virtual ~SmoothTriangle (void);
        virtual SmoothTriangle* clone(void) const;

        // Functions
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& s) const;
        // virtual BBox get_bounding_box(void);
        void compute_normal(void);
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

#endif