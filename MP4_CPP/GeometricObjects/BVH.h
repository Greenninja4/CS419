#ifndef __BVH__
#define __BVH__

#include "GeometricObject.h"
#include <vector>
#include "BBox.h"

using namespace std;

class BVH : public GeometricObject {
    public: 
        // Variables
        vector<GeometricObject*> objects;
        BBox bbox;
        GeometricObject* left;
        GeometricObject* right;

        // Big 6
        BVH(void);
        BVH(const BVH& bvh);
        BVH& operator=(const BVH& bvh);
        ~BVH(void);
        virtual BVH* clone(void) const;
        // IMPORTANT CONSTRUCTORS
        BVH(GeometricObject* left, GeometricObject* right);
        BVH(const vector<GeometricObject*>& objects, int num_objects, int axis);


        // Functions
        virtual void add_object(GeometricObject* object_ptr);
        void delete_objects(void);
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

// Non-member function
BBox surround(const vector<GeometricObject*>& objects);
bool compare0(GeometricObject* obj0, GeometricObject* obj1);
bool compare1(GeometricObject* obj0, GeometricObject* obj1);
bool compare2(GeometricObject* obj0, GeometricObject* obj1);
int qsplit(vector<GeometricObject*> objects, int num_objects, double pivot_point, int axis);

#endif

