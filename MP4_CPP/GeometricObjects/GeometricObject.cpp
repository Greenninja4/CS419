#include "GeometricObject.h"
#include "Material.h"
#include "Constants.h"

// Big 6
GeometricObject::GeometricObject(void): 
    material_ptr(NULL) {}
GeometricObject::GeometricObject(const GeometricObject& object){
    if (object.material_ptr){
        delete material_ptr;
        material_ptr = object.material_ptr->clone();
    }
}
GeometricObject& GeometricObject::operator= (const GeometricObject& rhs){
    if (this == &rhs){
        return *this;
    }
    if (material_ptr){
        delete material_ptr;
        material_ptr = NULL;
    }
    if (rhs.material_ptr){
        material_ptr = rhs.material_ptr->clone();
    }
    return *this;
}
GeometricObject::~GeometricObject(void){
    if (material_ptr){
        delete material_ptr;
        material_ptr = NULL;
    }
}

// Getters & Setters
Material* GeometricObject::get_material(void) const{
    return material_ptr;
}
void GeometricObject::set_material(Material* material_ptr){
    this->material_ptr = material_ptr;
}
BBox GeometricObject::get_bounding_box(void){
    return BBox();
}
void GeometricObject::set_bounding_box(void){}
Vector3D GeometricObject::get_normal(void) const{
    return Vector3D();
}
Vector3D GeometricObject::get_normal(const Vector3D& p) const{
    return Vector3D();
}

// Functions
// void GeometricObject::add_object(GeometricObject* object_ptr){}
Vector3D GeometricObject::sample(void){
    return Vector3D(0.0);
}
double GeometricObject::pdf(const ShadeRec& sr){
    return 0.0;
}
bool GeometricObject::shadow_hit(const Ray& ray, double& tmin) const{
    return false;
}

bool GeometricObject::min_hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    return false;
}