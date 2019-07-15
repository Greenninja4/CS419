#ifndef __SMOOTH_MESH__
#define __SMOOTH_MESH__

#include "GeometricObject.h"
#include "SmoothTriangle.h"
#include "BBox.h"
#include <string>
#include <vector>
#include "BVH.h"

using namespace std;

class SmoothMesh : public GeometricObject {
    public: 
        // Variables
        vector<Vector3D> vertices;
        vector<vector<int>> faces;
        vector<Vector3D> v_normals;
        vector<GeometricObject*> triangles;
        int num_vertices;
        int num_faces;
        BBox bbox;
        BVH bvh;

        // Big 6
        SmoothMesh(void);
        SmoothMesh(const string& file_name);
        SmoothMesh(const SmoothMesh& object);
        SmoothMesh& operator= (const SmoothMesh& rhs);
        virtual ~SmoothMesh(void);
        virtual SmoothMesh* clone(void) const;

        // Getters & Setters
        virtual BBox get_bounding_box(void); // Gets bounding box
        virtual void set_bounding_box(void); // Sets bounding box
        
        // Functions
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
};

#endif