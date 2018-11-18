#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "Sampler.h"
#include "GeometricObject.h"

class Rectangle : public GeometricObject{
    public:
        // Variables
        Vector3D p0;
        Vector3D a;
        Vector3D b;
        double a_len_squared;
        double b_len_squared;
        Vector3D normal;
        float area;
        float inv_area;
        Sampler* sampler_ptr;
        
        
        
        

        // Big 6
        Rectangle(void);
        Rectangle(const Vector3D& p0, const Vector3D& a, const Vector3D& b);
        Rectangle(const Vector3D& p0, const Vector3D& a, const Vector3D& b, Sampler* sampler_ptr);
        virtual Rectangle* clone() const;
        Rectangle(const Rectangle& rectangle);
        virtual ~Rectangle();
        Rectangle& operator= (const Rectangle& rhs);

        // Functions
        virtual Vector3D get_normal(void) const;
        virtual Vector3D get_normal(const Vector3D& p) const;
        BBox get_bounding_box();
        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual void set_sampler(Sampler* sampler);
        virtual Vector3D sample();
        virtual double pdf(const ShadeRec& sr);
};

#endif