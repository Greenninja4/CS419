#ifndef __INSTANCE__
#define __INSTANCE__

#include "GeometricObject.h"
#include "Matrix.h"

class Instance : public GeometricObject {
    public:
        //Variables
        GeometricObject* object_ptr;        // Original Object
        Matrix inv_matrix;                  // Inverse Transformation Matrix
        Matrix forward_matrix;              // Forward Transformation Matrix
        BBox bbox;
        bool transform_the_texture;         // Should we trasform the texture?

        // Big 6
        Instance(void);
        Instance(GeometricObject* object_ptr);
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

        // Updated functions
        void scale(const Vector3D& s);
        void rotate_x(const double theta);
        void rotate_y(const double theta);
        void rotate_z(const double theta);
};

#endif