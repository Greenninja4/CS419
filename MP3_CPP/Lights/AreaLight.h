#ifndef __AREA_LIGHT__
#define __AREA_LIGHT__

#include "Light.h"
#include "GeometricObject.h"
#include "Material.h"
#include "Vector3D.h"
#include "ShadeRec.h"

class AreaLight : public Light{
    public:
        // Variables
        GeometricObject* object_ptr;    // Area light shape
        Material* material_ptr;         // Material
        Vector3D sample_point;          // Sample point on the surface
        Vector3D light_normal;          // Normal at the sample point
        Vector3D wi;                    // Direction from hit point to sample point

        // Big 6
        AreaLight(void);
        AreaLight(GeometricObject* object_ptr);
        AreaLight(const AreaLight& area_light);
        virtual Light* clone() const;
        virtual ~AreaLight();
        AreaLight& operator= (const AreaLight& rhs);

        // Functions
        void set_object(GeometricObject* object_ptr);
        virtual Vector3D get_direction(ShadeRec& sr);
        virtual Vector3D L(ShadeRec& sr);
        virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;
        virtual float G(const ShadeRec& sr) const;
        virtual float pdf(const ShadeRec& sr) const;
        virtual bool casts_shadow(void) const;
};

#endif