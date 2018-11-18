#include "Rectangle.h"

// Big 6
Rectangle::Rectangle(void):
    GeometricObject(),
    p0(-1,0,-1),
    a(0,0,2),
    b(2,0,0),
    a_len_squared(4),
    b_len_squared(4),
    normal(0,1,0),
    area(4),
    inv_area(.25),
    sampler_ptr(NULL) {}
Rectangle::Rectangle(const Vector3D& p0, const Vector3D& a, const Vector3D& b):
    GeometricObject(),
    p0(p0),
    a(a),
    b(b),
    a_len_squared(this->a.length() * this->a.length()),
    b_len_squared(this->b.length() * this->b.length()),
    area(this->a.length() * this->b.length()),
    inv_area(1/area),
    sampler_ptr(NULL) {
    normal = a^b;
    normal.normalize();
}
Rectangle::Rectangle(const Vector3D& p0, const Vector3D& a, const Vector3D& b, Sampler* sampler_ptr):
    GeometricObject(),
    p0(p0),
    a(a),
    b(b),
    a_len_squared(this->a.length() * this->a.length()),
    b_len_squared(this->b.length() * this->b.length()),
    area(this->a.length() * this->b.length()),
    inv_area(1/area),
    sampler_ptr(sampler_ptr) {
    normal = a^b;
    normal.normalize();
}
Rectangle::Rectangle(const Rectangle& rectangle):
    GeometricObject(rectangle),
    p0(rectangle.p0),
    a(rectangle.a),
    b(rectangle.b),
    a_len_squared(rectangle.a_len_squared),
    b_len_squared(rectangle.b_len_squared),
    area(rectangle.area),
    inv_area(rectangle.inv_area){
    if (rectangle.sampler_ptr){
        sampler_ptr = rectangle.sampler_ptr->clone(); 
    } else {
        sampler_ptr = NULL;
    }
}
Rectangle* Rectangle::clone() const{
    return (new Rectangle(*this));
}
Rectangle::~Rectangle(){
    if (sampler_ptr){
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
}
Rectangle& Rectangle::operator= (const Rectangle& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator=(rhs);
    p0 = rhs.p0;
    a = rhs.a;
    b = rhs.b;
    a_len_squared = rhs.a_len_squared;
    b_len_squared = rhs.b_len_squared;
    area = rhs.area;
    inv_area = rhs.inv_area;
    normal = rhs.normal;

    if(sampler_ptr){
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    if (rhs.sampler_ptr){
        sampler_ptr = rhs.sampler_ptr->clone();
    }
    return *this;
}

// Functions
Vector3D Rectangle::get_normal(void) const{
    return normal;
}
Vector3D Rectangle::get_normal(const Vector3D& p) const{
    return normal;
}
BBox Rectangle::get_bounding_box(){
    return(BBox(min(p0.x, p0.x + a.x + b.x) - KEPSILON, max(p0.x, p0.x + a.x + b.x) + KEPSILON,
                min(p0.y, p0.y + a.y + b.y) - KEPSILON, max(p0.y, p0.y + a.y + b.y) + KEPSILON,
                min(p0.z, p0.z + a.z + b.z) - KEPSILON, max(p0.z, p0.z + a.z + b.z) + KEPSILON));
}
bool Rectangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    double t = (p0 - ray.o) * normal / (ray.d * normal);

    if (t <= KEPSILON){
        return false;
    }

    Vector3D p = ray.o + t * ray.d;
    Vector3D d = p - p0;

    double ddota = d * a;
    double ddotb = d * b;
    if (ddota < 0.0 || ddota > a_len_squared){
        return false;
    }
    if (ddotb < 0.0 || ddotb > b_len_squared){
        return false;
    }

    tmin = t;
    sr.normal = normal;
    sr.local_hit_point = p;
    return true;
}

bool Rectangle::shadow_hit(const Ray& ray, double& tmin) const{
    double t = (p0 - ray.o) * normal / (ray.d * normal);

    if (t <= KEPSILON){
        return false;
    }

    Vector3D p = ray.o + t * ray.d;
    Vector3D d = p - p0;

    double ddota = d * a;
    double ddotb = d * b;
    if (ddota < 0.0 || ddota > a_len_squared){
        return false;
    }
    if (ddotb < 0.0 || ddotb > b_len_squared){
        return false;
    }

    tmin = t;
    return true;
}

void Rectangle::set_sampler(Sampler* sampler){
    sampler_ptr = sampler;
}
Vector3D Rectangle::sample(){
    Vector3D sample_point = sampler_ptr->sample_unit_square();
    return (p0 + sample_point.x * a + sample_point.y * b);
}
double Rectangle::pdf(const ShadeRec& sr){
    return inv_area;
}