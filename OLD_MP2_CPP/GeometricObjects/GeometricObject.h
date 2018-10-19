#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

#include "Vector3D.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "BBox.h"

class Material;

class GeometricObject{
    public:
        mutable Material* material_ptr;

        GeometricObject(void);
        GeometricObject(Material* material_ptr);
        GeometricObject(const GeometricObject& object);
        GeometricObject& operator=(const GeometricObject& rhs);
        virtual ~GeometricObject(void);
        virtual GeometricObject* clone(void) const = 0;

		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
        Material* get_material(void) const;
        virtual void set_material(Material* material_ptr);

        virtual BBox get_bounding_box(void);
        virtual void add_object(GeometricObject* object_ptr);
        Vector3D get_normal(void) const;
};

#endif