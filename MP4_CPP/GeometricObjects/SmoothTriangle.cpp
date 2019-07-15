#include "SmoothTriangle.h"
#include "Constants.h"
#include <math.h>

// Big 6
SmoothTriangle::SmoothTriangle(void): 
    GeometricObject(), 
    p1(0.0, 0.0, 0.0), 
    p2(0.0, 0.0, 1.0), 
    p3(1.0, 0.0, 0.0), 
    n(0.0, 1.0, 0.0),
    n1(0.0, 1.0, 0.0),
    n2(0.0, 1.0, 0.0),
    n3(0.0, 1.0, 0.0) {
    set_bounding_box();
}
SmoothTriangle::SmoothTriangle(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& n1, const Vector3D& n2, const Vector3D& n3): 
    GeometricObject(), 
    p1(p1), 
    p2(p2), 
    p3(p3),
    n1(n1),
    n2(n2),
    n3(n3) {
    compute_normal();
    set_bounding_box();
}
SmoothTriangle::SmoothTriangle(const SmoothTriangle& tri): 
    GeometricObject(tri), 
    p1(tri.p1), 
    p2(tri.p2), 
    p3(tri.p3), 
    n(tri.n),
    n1(tri.n1), 
    n2(tri.n2),
    n3(tri.n3),
    bbox(tri.bbox) {}
SmoothTriangle& SmoothTriangle::operator= (const SmoothTriangle& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    p1 = rhs.p1;
    p2 = rhs.p2;
    p3 = rhs.p3;
    n = rhs.n;
    n1 = rhs.n1;
    n2 = rhs.n2;
    n3 = rhs.n3;
    return *this;
}
SmoothTriangle::~SmoothTriangle (void){}
SmoothTriangle* SmoothTriangle::clone(void) const{
    return (new SmoothTriangle(*this));
}

// Functions
bool SmoothTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t = ((p1 - ray.o) * n) / (ray.d * n);
    if (t > KEPSILON){
        // Calculate barycentric coordinates
        double temp, b1, b2, b3;
        Vector3D hit_point = ray.o + t * ray.d;
        temp = ((p2 - p1)^(p3-p1)) * n;
        b1 = ((p3 - p2)^(hit_point - p2)) * n / temp;
        b2 = ((p1 - p3)^(hit_point - p3)) * n / temp;
        b3 = 1 - b1 - b2; // b3 = ((p2 - p1)^(hit_point - p1)) * n / temp;

        if (0 < b1 && b1 < 1 && 0 < b2 && b2 < 1 && 0 < b3 && b3 < 1){
            tmin = t;
            // Find smoothed normal
            sr.normal = b1*n1 + b2*n2 + b3*n3;
            sr.local_hit_point = hit_point;
            return true;
        }
    }
    return false;
}
// BBox SmoothTriangle::get_bounding_box(void){
//     return (BBox(MIN(MIN(p1.x, p2.x), p3.x) - KEPSILON, MAX(MAX(p1.x, p2.x), p3.x) + KEPSILON,
//                  MIN(MIN(p1.y, p2.y), p3.y) - KEPSILON, MAX(MAX(p1.y, p2.y), p3.y) + KEPSILON,
//                  MIN(MIN(p1.z, p2.z), p3.z) - KEPSILON, MAX(MAX(p1.z, p2.z), p3.z) + KEPSILON));
// }
void SmoothTriangle::compute_normal(void) {
    n = (p2 - p1) ^ (p3 - p1);
    n.normalize();
}
bool SmoothTriangle::shadow_hit(const Ray& ray, double& tmin) const {
    double t = ((p1 - ray.o) * n) / (ray.d * n);
    if (t > KEPSILON){
        // Calculate barycentric coordinates
        double temp, b1, b2, b3;
        Vector3D hit_point = ray.o + t * ray.d;
        temp = ((p2 - p1)^(p3-p1)) * n;
        b1 = ((p3 - p2)^(hit_point - p2)) * n / temp;
        b2 = ((p1 - p3)^(hit_point - p3)) * n / temp;
        b3 = 1 - b1 - b2; // b3 = ((p2 - p1)^(hit_point - p1)) * n / temp;

        if (0 < b1 && b1 < 1 && 0 < b2 && b2 < 1 && 0 < b3 && b3 < 1){
            tmin = t;
            return true;
        }
    }
    return false;
}

BBox SmoothTriangle::get_bounding_box(void){
    return bbox;
}
void SmoothTriangle::set_bounding_box(void){
    double x0 = MIN(MIN(p1.x, p2.x), p3.x);
    double y0 = MIN(MIN(p1.y, p2.y), p3.y);
    double z0 = MIN(MIN(p1.z, p2.z), p3.z);
    double x1 = MAX(MAX(p1.x, p2.x), p3.x);
    double y1 = MAX(MAX(p1.y, p2.y), p3.y);
    double z1 = MAX(MAX(p1.z, p2.z), p3.z);
    bbox = BBox(Vector3D(x0, y0, z0), Vector3D(x1, y1, z1));
}