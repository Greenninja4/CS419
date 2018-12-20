#ifndef __FAST_TRI_MESH__
#define __FAST_TRI_MESH__

#include "GeometricObject.h"
#include "Triangle.h"
#include "BBox.h"
#include <string>
#include <vector>
#include "BVH.h"

using namespace std;

class FastTriMesh : public GeometricObject {
    public: 
        // Variables
        vector<Vector3D> vertices;
        vector<vector<int>> faces;
        vector<GeometricObject*> triangles;
        int num_vertices;
        int num_faces;
        BBox bbox;
        BVH bvh;

        // Big 6
        FastTriMesh(void);
        FastTriMesh(const string& file_name);
        FastTriMesh(const FastTriMesh& object);
        FastTriMesh& operator= (const FastTriMesh& rhs);
        virtual ~FastTriMesh(void);
        virtual FastTriMesh* clone(void) const;

        // Getters & Setters
        virtual BBox get_bounding_box(void); // Gets bounding box
        virtual void set_bounding_box(void); // Sets bounding box
        
        // Functions
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
};

#endif