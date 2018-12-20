#ifndef __DEFORMED__
#define __DEFORMED__

#include "GeometricObject.h"
#include "Triangle.h"
#include "BBox.h"
#include <string>
#include <vector>
#include "BVH.h"

using namespace std;

class Deformed : public GeometricObject {
    public: 
        // Variables
        vector<Vector3D> vertices;
        vector<vector<int>> faces;
        vector<GeometricObject*> triangles;
        int num_vertices;
        int num_faces;
        BBox bbox;
        BVH bvh;
        vector<Vector3D> def_vertices;
        vector<vector<int>> def_faces;
        vector<GeometricObject*> def_triangles;

        // Big 6
        Deformed(void);
        Deformed(const string& file_name);
        Deformed(const Deformed& object);
        Deformed& operator= (const Deformed& rhs);
        virtual ~Deformed(void);
        virtual Deformed* clone(void) const;

        // Getters & Setters
        virtual BBox get_bounding_box(void); // Gets bounding box
        virtual void set_bounding_box(void); // Sets bounding box
        
        // Functions
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        void balloon(float rate);
        void recalculate_triangles(void);
};

#endif