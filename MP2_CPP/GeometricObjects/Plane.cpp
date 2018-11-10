#include "Plane.h"
#include "Constants.h"

// Big 6
Plane::Plane(void): 
    GeometricObject(), 
    p(0.0), 
    n(0.0, 1.0, 0.0) {
        set_bounding_box();
    }
Plane::Plane(const Vector3D& p, const Vector3D& n): 
    GeometricObject(), 
    p(p),
    n(n)
    {this->n.normalize();
    set_bounding_box();
}
Plane::Plane(const Plane& plane):
    GeometricObject(plane), 
    p(plane.p), 
    n(plane.n) {
        set_bounding_box();
    }
Plane& Plane::operator= (const Plane& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    p = rhs.p;
    n = rhs.n;
    return *this;
}
Plane::~Plane(void){}
Plane* Plane::clone(void) const{
    return (new Plane(*this));
}

// Functions
bool Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t = (p - ray.o) * n / (ray.d * n);
    if (t > KEPSILON){
        tmin = t;
        sr.normal = n;
        sr.local_hit_point = ray.o + t * ray.d;
        return true;
    } else {
        return false;
    }
}
bool Plane::shadow_hit(const Ray& ray, double& tmin) const{
    double t = (p - ray.o) * n / (ray.d * n);
    if (t > KEPSILON){
        tmin = t;
        return true;
    } else {
        return false;
    }
}

BBox Plane::get_bounding_box(void){
    return bbox;
}
void Plane::set_bounding_box(void){
    bbox = BBox(Vector3D(-KHUGEVALUE), Vector3D(KHUGEVALUE));
}