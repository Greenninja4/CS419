#ifndef __TRIANGLE_MESH__
#define __TRIANGLE_MESH__

#include "GeometricObject.h"
#include "Triangle.h"
#include "BBox.h"
#include <string>
#include <vector>

using namespace std;

class TriangleMesh : public GeometricObject {
    public: 
        // Variables
        vector<Vector3D> vertices;
        vector<vector<int>> faces;
        vector<Triangle*> triangles;
        int num_vertices;
        int num_faces;
        BBox bbox;

        // Big 6
        TriangleMesh(void);
        TriangleMesh(const string& file_name);
        TriangleMesh(const TriangleMesh& object);
        TriangleMesh& operator= (const TriangleMesh& rhs);
        virtual ~TriangleMesh(void);
        virtual TriangleMesh* clone(void) const;

        // Getters & Setters
        virtual BBox get_bounding_box(void); // Gets bounding box
        virtual void set_bounding_box(void); // Sets bounding box
        
        // Functions
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
};

#endif