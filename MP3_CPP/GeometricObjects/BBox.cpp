#include "BBox.h"
#include "Constants.h"

// Big 6
BBox::BBox(void): 
    p0(-1, 1, -1), 
    p1(1, -1, 1) {}
BBox::BBox(const double& x0, const double& y0, const double& z0, 
           const double& x1, const double& y1, const double& z1): 
    p0(x0, y0, z0), 
    p1(x1, y1, z1) {}
BBox::BBox(const Vector3D& p0, const Vector3D& p1): 
    p0(p0),
    p1(p1) {}
BBox::BBox(const BBox& bbox): 
    p0(bbox.p0), 
    p1(bbox.p1) {}
BBox& BBox::operator=(const BBox& rhs){
    if (this == &rhs){
        return *this;
    }
    p0 = rhs.p0;
    p1 = rhs.p1;
    return *this;
}
BBox::~BBox(void){}
BBox* BBox::clone(void) const{
    return (new BBox(*this));
}

// Functions
bool BBox::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    double ox = ray.o.x;
    double oy = ray.o.y;
    double oz = ray.o.z;
    double dx = ray.d.x;
    double dy = ray.d.y;
    double dz = ray.d.z;

    double tx_min, ty_min, tz_min;
    double tx_max, ty_max, tz_max;

    double a = 1.0 / dx;
    if (a >= 0) {
        tx_min = (p0.x - ox) * a;
        tx_max = (p1.x - ox) * a;
    } else {
        tx_min = (p1.x - ox) * a;
        tx_max = (p0.x - ox) * a;
    }

    double b = 1.0 / dy;
    if (b >= 0) {
        ty_min = (p0.y - oy) * b;
        ty_max = (p1.y - oy) * b;
    } else {
        ty_min = (p1.y - oy) * b;
        ty_max = (p0.y - oy) * b;
    }

    double c = 1.0 / dz;
    if (c >= 0) {
        tz_min = (p0.z - oz) * c;
        tz_max = (p1.z - oz) * c;
    } else {
        tz_min = (p1.z - oz) * c;
        tz_max = (p0.z - oz) * c;
    }

    double t0, t1;

    // find largest entering t value

    if (tx_min > ty_min)
        t0 = tx_min;
    else
        t0 = ty_min;

    if (tz_min > t0)
        t0 = tz_min;

    // find smallest exiting t value

    if (tx_max < ty_max)
        t1 = tx_max;
    else
        t1 = ty_max;

    if (tz_max < t1)
        t1 = tz_max;

    return (t0 < t1 && t1 > KEPSILON);
}
bool BBox::inside(const Vector3D& p){
    return ( (p.x > p0.x && p.x < p1.x) && (p.y > p0.y && p.y < p1.y) && (p.z > p0.z && p.z < p1.z) );
}
bool BBox::shadow_hit(const Ray& ray, double& tmin) const{
    double ox = ray.o.x;
    double oy = ray.o.y;
    double oz = ray.o.z;
    double dx = ray.d.x;
    double dy = ray.d.y;
    double dz = ray.d.z;

    double tx_min, ty_min, tz_min;
    double tx_max, ty_max, tz_max;

    double a = 1.0 / dx;
    if (a >= 0) {
        tx_min = (p0.x - ox) * a;
        tx_max = (p1.x - ox) * a;
    } else {
        tx_min = (p1.x - ox) * a;
        tx_max = (p0.x - ox) * a;
    }

    double b = 1.0 / dy;
    if (b >= 0) {
        ty_min = (p0.y - oy) * b;
        ty_max = (p1.y - oy) * b;
    } else {
        ty_min = (p1.y - oy) * b;
        ty_max = (p0.y - oy) * b;
    }

    double c = 1.0 / dz;
    if (c >= 0) {
        tz_min = (p0.z - oz) * c;
        tz_max = (p1.z - oz) * c;
    } else {
        tz_min = (p1.z - oz) * c;
        tz_max = (p0.z - oz) * c;
    }

    double t0, t1;

    // find largest entering t value

    if (tx_min > ty_min)
        t0 = tx_min;
    else
        t0 = ty_min;

    if (tz_min > t0)
        t0 = tz_min;

    // find smallest exiting t value

    if (tx_max < ty_max)
        t1 = tx_max;
    else
        t1 = ty_max;

    if (tz_max < t1)
        t1 = tz_max;

    return (t0 < t1 && t1 > KEPSILON);
}