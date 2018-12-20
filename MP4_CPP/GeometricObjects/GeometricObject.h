#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

#include <math.h>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "BBox.h"
#include <iostream>

class Material; // Material is only a pointer, so we can use forward declaration

class GeometricObject{
    public:
        // Variables
        mutable Material* material_ptr; // Material

        // Big 6
        GeometricObject(void);
        GeometricObject(const GeometricObject& object);
        GeometricObject& operator= (const GeometricObject& rhs);
        virtual ~GeometricObject(void);
        virtual GeometricObject* clone(void) const = 0;

        // Getters & Setters
        Material* get_material(void) const;                                     // Get the material
        virtual void set_material(Material* material_ptr);                      // Set the material
        virtual BBox get_bounding_box(void);                                    // Gets bounding box
        virtual void set_bounding_box(void);                                    // Sets bounding box
        virtual Vector3D get_normal(void) const;                                // Get the normal
        virtual Vector3D get_normal(const Vector3D& p) const;                   // Get the normal at a point                                    

        // Functions
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0; // Ray hits the object?
        virtual bool min_hit(const Ray& ray, double& tmin, ShadeRec& sr) const; // For CSG Model Spheres
        // virtual void add_object(GeometricObject* object_ptr);                   // For compound objects... Not needed rn
        virtual Vector3D sample(void);                                          // For area lights
        virtual double pdf(const ShadeRec& sr);                                 // For area lights
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
};

#endif