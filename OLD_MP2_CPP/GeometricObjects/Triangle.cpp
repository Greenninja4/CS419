#include "Triangle.h"

#include "Constants.h"
#include <Maths.h>

#include <cmath>
using std::min;
using std::max;

Triangle::Triangle(void): 
    GeometricObject(), 
    p1(1.0, 0.0, 0.0),
    p2(0.0, 1.0, 0.0),
    p3(0.0, 0.0, 1.0), 
    n(0, 1, 0) {
        Vector3D a = p1 - p2;
        Vector3D b = p1 - p3;
        n = Vector3D(a^b);
        n.normalize();
}
Triangle::Triangle(Vector3D p1, Vector3D p2, Vector3D p3): 
    GeometricObject(), 
    p1(p1),
    p2(p2),
    p3(p3), 
    n(0, 1, 0) {
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







// ---------------------------------------------------------------- compute_normal

void
Triangle::compute_normal(void) {
    n = (p2 - p1) ^ (p3 - p1);
    n.normalize();
}


BBox
Triangle::get_bounding_box(void) {
    double delta = 0.000001;

    return (BBox(min(min(p1.x, p2.x), p3.x) - delta, max(max(p1.x, p2.x), p3.x) + delta,
                 min(min(p1.y, p2.y), p3.y) - delta, max(max(p1.y, p2.y), p3.y) + delta,
                 min(min(p1.z, p2.z), p3.z) - delta, max(max(p1.z, p2.z), p3.z) + delta));
}
bool
Triangle::shadow_hit(const Ray& ray, double& tmin) const {
    double a = p1.x - p2.x, b = p1.x - p3.x, c = ray.d.x, d = p1.x - ray.o.x;
    double e = p1.y - p2.y, f = p1.y - p3.y, g = ray.d.y, h = p1.y - ray.o.y;
    double i = p1.z - p2.z, j = p1.z - p3.z, k = ray.d.z, l = p1.z - ray.o.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, s = e * j - f * i;

    double inv_denom  = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0)
        return (false);

    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;

    if (gamma < 0.0)
        return (false);

    if (beta + gamma > 1.0)
        return (false);

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < kEpsilon)
        return (false);

    tmin = t;

    return(true);
}