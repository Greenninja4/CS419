#ifndef __INSTANCE__
#define __INSTANCE__

#include "GeometricObject.h"
#include "Matrix.h"

class Instance : public GeometricObject {
    public:
        //Variables
        GeometricObject* object_ptr;        // Original Object
        Matrix inv_matrix;                  // Inverse Transformation Matrix
        bool transform_the_texture;         // Should we trasform the texture?
        BBox bbox;

        // Big 6
        Instance(void);
        Instance(const GeometricObject* object_ptr);
        Instance(const Instance& instance);
        Instance& operator= (const Instance& rhs);
        virtual ~Instance(void);
        virtual Instance* clone(void) const;

        // Functions
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        void translate(const Vector3D& v);
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

#endif