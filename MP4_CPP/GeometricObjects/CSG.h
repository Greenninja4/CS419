#ifndef __CSG__
#define __CSG__

#include "GeometricObject.h"
#include <vector>
#include "Material.h"

class CSG : public GeometricObject{
    public:
        // Variables
        GeometricObject* left;
        GeometricObject* right;
        int int_uni_min; // 0 = intersection, 1 = union, 2 = minus
        BBox bbox;

        // Big 6
        CSG(void);
        CSG(GeometricObject* left, GeometricObject* right, int int_uni_min);
        CSG(const CSG& csg);
        CSG& operator= (const CSG& rhs);
        ~CSG (void);
        virtual CSG* clone(void) const;

        // Functions
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};
#endif