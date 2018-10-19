#ifndef __MESH_TRIANGLE__
#define __MESH_TRIANGLE__

#include "GeometricObject.h"
#include "Mesh.h"
#include "BBox.h"

class MeshTriangle : public GeometricObject {
    public:
        Mesh* mesh_ptr;
        int index0, index1, index2;
        Vector3D normal;
        float area;

        MeshTriangle(void);
        MeshTriangle(Mesh* mesh_ptr, int i1, int i2, int i3);
        virtual MeshTriangle* clone(void) const = 0;
        MeshTriangle(const MeshTriangle& mt);
        virtual ~MeshTriangle(void);
        MeshTriangle& operator=(const MeshTriangle& rhs);
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        void compute_normal(const bool reverse_normal);
        virtual Vector3D get_normal(void) const;
        virtual BBox get_bounding_box(void);
        float interpolate_u(const float beta, const float gamma) const;
        float interpolate_v(const float beta, const float gamma) const;
};

#endif