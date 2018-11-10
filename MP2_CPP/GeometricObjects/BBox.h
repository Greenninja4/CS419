#ifndef __BBOX__
#define __BBOX__

#include "Ray.h"
#include "Vector3D.h"
#include "ShadeRec.h"
#include <vector>

class GeometricObject;

using namespace std;

class BBox {
    public:
        // Variables
        Vector3D p0, p1;

        // Big 6
        BBox(void);
        BBox(const double& x0, const double& y0, const double& z0, 
             const double& x1, const double& y1, const double& z1);
        BBox(const Vector3D& p0, const Vector3D& p1);
        BBox(const vector<GeometricObject*> objects);
        BBox(const BBox& bbox);
        BBox& operator=(const BBox& rhs);
        ~BBox(void);
        BBox* clone(void) const;


        // Functions
        bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        bool inside(const Vector3D& p);
        bool shadow_hit(const Ray& ray, double& tmin) const;
};

#endif