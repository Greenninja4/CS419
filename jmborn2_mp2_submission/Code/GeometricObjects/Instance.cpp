#include "Instance.h"

// Big 6
Instance::Instance(void){}
Instance::Instance(const GeometricObject* object_ptr){}
Instance::Instance(const Instance& instance){}
Instance& Instance::operator= (const Instance& rhs){
    return *this;
}
Instance::~Instance(void){}
Instance* Instance::clone(void) const{
    return (new Instance(*this));
}

// Functions
bool Instance::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    Ray inv_ray(ray);
    inv_ray.o = inv_matrix * inv_ray.o;
    inv_ray.d = inv_matrix * inv_ray.d;
    if (object_ptr->hit(inv_ray, tmin, sr)){
        sr.normal = inv_matrix ^ sr.normal;
        sr.normal.normalize();
        if (object_ptr->get_material()){
            material_ptr = object_ptr->get_material();
        }
        if (!transform_the_texture){
            sr.local_hit_point = ray.o + tmin * ray.d;
        }
        return true;
    }
    return false;
}
bool Instance::shadow_hit(const Ray& ray, double& tmin) const{
    Ray inv_ray(ray);
    inv_ray.o = inv_matrix * inv_ray.o;
    inv_ray.d = inv_matrix * inv_ray.d;
    if (object_ptr->shadow_hit(inv_ray, tmin)){
        return true;
    }
    return false;
}
void Instance::translate(const Vector3D& v){
    Matrix inv_translation_matrix;
    inv_translation_matrix.m[0][3] = -v.x;
    inv_translation_matrix.m[1][3] = -v.y;
    inv_translation_matrix.m[2][3] = -v.z;
    inv_matrix = inv_matrix * inv_translation_matrix;
}

BBox Instance::get_bounding_box(void){
    return bbox;
}
void Instance::set_bounding_box(void){
    BBox obj_bbox = object_ptr->get_bounding_box();
    Vector3D p0 = inv_matrix * obj_bbox.p0;
    Vector3D p1 = inv_matrix * obj_bbox.p1;
    bbox = BBox(p0, p1);
}