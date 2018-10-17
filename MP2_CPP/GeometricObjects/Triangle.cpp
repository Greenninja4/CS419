#include "Triangle.h"

#include "Constants.h"
#include <cmath>

Triangle::Triangle(void): 
    GeometricObject(), 
    p1(1.0, 0.0, 0.0),
    p2(0.0, 1.0, 0.0),
    p3(0.0, 0.0, 1.0)
    {
        Vector3D a = p1 - p2;
        Vector3D b = p1 - p3;
        n = a^b;
        n.normalize();
    }
Triangle::Triangle(Vector3D p1, Vector3D p2, Vector3D p3): 
    GeometricObject(), 
    p1(p1),
    p2(p2),
    p3(p3)
    {
        Vector3D a = p1 - p2;
        Vector3D b = p1 - p3;
        n = a^b;
        n.normalize();
    }
Triangle::Triangle(const Triangle& triangle): 
    GeometricObject(triangle), 
    p1(triangle.p1), 
    p2(triangle.p2), 
    p3(triangle.p3), 
    n(triangle.n) {}
Triangle& Triangle::operator=(const Triangle& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator=(rhs);
    p1 = rhs.p1;
    p2 = rhs.p2;
    p3 = rhs.p3;
    n = rhs.n;
    return *this;
}
Triangle::~Triangle (void){}
Triangle* Triangle::clone(void) const{
    return (new Triangle(*this));
}
// Hit function
bool Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t = ((p1 - ray.o) * n) / (ray.d * n);
    // Hits plane
    if (t > KEPSILON){
        // Calculate barycentric coordinates
        double temp, b1, b2, b3;
        Vector3D hitPoint = ray.o + t * ray.d;
        temp = ((p2 - p1)^(p3-p1)) * n;
        b1 = ((p3 - p2)^(hitPoint - p2)) * n / temp;
        b2 = ((p1 - p3)^(hitPoint - p3)) * n / temp;
        // Not needed b/c how barycentric works
        // b3 = ((p2 - p1)^(hitPoint - p1)) * n / temp;
        b3 = 1 - b1 - b2;

        // Hits triangle?
        if (0 < b1 && b1 < 1 && 0 < b2 && b2 < 1 && 0 < b3 && b3 < 1){
            tmin = t;
            sr.normal = n;
            sr.localHitPoint = hitPoint;
            return true;
        }
    }
    return false;
}
