#include "Instance.h"

// Big 6
Instance::Instance(void):
    GeometricObject(),
    object_ptr(NULL),
    inv_matrix(),
    forward_matrix(),
    bbox(),
    transform_the_texture(true) {}
Instance::Instance(GeometricObject* object_ptr):
    GeometricObject(),
    object_ptr(object_ptr),
    inv_matrix(),
    forward_matrix(),
    bbox(),
    transform_the_texture(true) {}
Instance::Instance(const Instance& instance):
    GeometricObject(instance),
    object_ptr(instance.object_ptr),
    inv_matrix(instance.inv_matrix),
    forward_matrix(instance.forward_matrix),
    bbox(instance.bbox),
    transform_the_texture(instance.transform_the_texture) {}
Instance& Instance::operator= (const Instance& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator=(rhs);
    if (object_ptr){
        delete object_ptr;
        object_ptr = NULL;
    }
    if (rhs.object_ptr){
        object_ptr = rhs.object_ptr->clone();
    } else {
        object_ptr = NULL;
    }
    inv_matrix = rhs.inv_matrix;
    forward_matrix = rhs.forward_matrix;
    bbox = rhs.bbox;
    transform_the_texture = rhs.transform_the_texture;
    return *this;
}
Instance::~Instance(void){
    if (object_ptr){
        delete object_ptr;
        object_ptr = NULL;
    }
}
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
        // if (object_ptr->get_material()){
        //     material_ptr = object_ptr->get_material();
        // }
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

BBox Instance::get_bounding_box(void){
    return bbox;
}
void Instance::set_bounding_box(void){
    BBox obj_bbox = object_ptr->get_bounding_box();
    // Vector3D p0 = inv_matrix * obj_bbox.p0;
    // Vector3D p1 = inv_matrix * obj_bbox.p1;
    Vector3D p0 = forward_matrix * obj_bbox.p0;
    Vector3D p1 = forward_matrix * obj_bbox.p1;
    bbox = BBox(p0, p1);
}

void Instance::translate(const Vector3D& v){
    Matrix translation_matrix;
    translation_matrix.m[0][3] = v.x;
    translation_matrix.m[1][3] = v.y;
    translation_matrix.m[2][3] = v.z;
    forward_matrix = translation_matrix * forward_matrix;

    Matrix inv_translation_matrix;
    inv_translation_matrix.m[0][3] = -v.x;
    inv_translation_matrix.m[1][3] = -v.y;
    inv_translation_matrix.m[2][3] = -v.z;
    inv_matrix = inv_matrix * inv_translation_matrix;
}

void Instance::scale(const Vector3D& s){
    Matrix scaling_matrix;
    scaling_matrix.m[0][0] = s.x;
    scaling_matrix.m[1][1] = s.y;
    scaling_matrix.m[2][2] = s.z;
    forward_matrix = scaling_matrix * forward_matrix;

    Matrix inv_scaling_matrix;
    inv_scaling_matrix.m[0][0] = 1.0/s.x;
    inv_scaling_matrix.m[1][1] = 1.0/s.y;
    inv_scaling_matrix.m[2][2] = 1.0/s.z;
    inv_matrix = inv_matrix * inv_scaling_matrix;
}
void Instance::rotate_x(const double theta){
    Matrix rot_matrix;
    rot_matrix.m[1][1] = cos(theta);
    rot_matrix.m[1][2] = -sin(theta);
    rot_matrix.m[2][1] = sin(theta);
    rot_matrix.m[2][2] = cos(theta);
    forward_matrix = rot_matrix * forward_matrix;

    Matrix inv_rot_matrix;
    inv_rot_matrix.m[1][1] = cos(theta);
    inv_rot_matrix.m[1][2] = sin(theta);
    inv_rot_matrix.m[2][1] = -sin(theta);
    inv_rot_matrix.m[2][2] = cos(theta);
    inv_matrix = inv_matrix * inv_rot_matrix;

}
void Instance::rotate_y(const double theta){
    Matrix rot_matrix;
    rot_matrix.m[0][0] = cos(theta);
    rot_matrix.m[0][2] = sin(theta);
    rot_matrix.m[2][0] = -sin(theta);
    rot_matrix.m[2][2] = cos(theta);
    rot_matrix.set_to_rot(theta, "y");
    forward_matrix = rot_matrix * forward_matrix;

    Matrix inv_rot_matrix;
    inv_rot_matrix.m[0][0] = cos(theta);
    inv_rot_matrix.m[0][2] = -sin(theta);
    inv_rot_matrix.m[2][0] = sin(theta);
    inv_rot_matrix.m[2][2] = cos(theta);
    rot_matrix.set_to_rot_inv(theta, "y");
    inv_matrix = inv_matrix * inv_rot_matrix;

}
void Instance::rotate_z(const double theta){
    Matrix rot_matrix;
    rot_matrix.m[0][0] = cos(theta);
    rot_matrix.m[0][1] = -sin(theta);
    rot_matrix.m[1][0] = sin(theta);
    rot_matrix.m[1][1] = cos(theta);
    forward_matrix = rot_matrix * forward_matrix;

    Matrix inv_rot_matrix;
    inv_rot_matrix.m[0][0] = cos(theta);
    inv_rot_matrix.m[0][1] = sin(theta);
    inv_rot_matrix.m[1][0] = -sin(theta);
    inv_rot_matrix.m[1][1] = cos(theta);
    inv_matrix = inv_matrix * inv_rot_matrix;

}