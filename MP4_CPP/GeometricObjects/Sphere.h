#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"

class Sphere : public GeometricObject{
    public:
        // Variables
        Vector3D center;    // Center
        double radius;      // Radius
        BBox bbox;          // Bounding box

        // Big 6
        Sphere(void);
        Sphere(const Vector3D& center, const double& radius);
        Sphere(const Sphere& sphere);
        Sphere& operator= (const Sphere& rhs);
        virtual ~Sphere (void);
        virtual Sphere* clone(void) const;

        // Getters & Setters
        void set_center(const Vector3D& center);
        void set_radius(const double& radius);

        // Functions
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
        virtual bool min_hit(const Ray& ray, double& t, ShadeRec& s) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};

#endif