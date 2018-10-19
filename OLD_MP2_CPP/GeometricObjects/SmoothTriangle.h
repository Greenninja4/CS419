#ifndef __SMOOTH_TRIANGLE__
#define __SMOOTH_TRIANGLE__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This triangle has three normals which are linearly interpolated at the hit point
// for smooth shading
// Making the normals public simplifies the code in the Grid::tessellate_smooth_sphere. This is currently
// the only function that uses this class. The only figure that uses it is Figure 23.3

#include "GeometricObject.h"

class SmoothTriangle: public GeometricObject {
public:

    Vector3D n0, n1, n2;		// normals at each vertex

    SmoothTriangle (void);

    SmoothTriangle (const Vector3D& a, const Vector3D& b, const Vector3D& c);

    virtual SmoothTriangle*
    clone(void) const;

    SmoothTriangle(const SmoothTriangle& st);

    virtual
    ~SmoothTriangle(void);

    SmoothTriangle&
    operator= (const SmoothTriangle& rhs);

    virtual BBox
    get_bounding_box(void);

    virtual bool
    hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

    virtual bool
    shadow_hit(const Ray& ray, double& tmin) const;

private:
    Vector3D v0, v1, v2;

    Vector3D interpolate_normal(const float beta, const float gamma) const;
};

#endif