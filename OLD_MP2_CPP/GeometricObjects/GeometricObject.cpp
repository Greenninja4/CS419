#include "GeometricObject.h"

#include "Material.h"
#include "Constants.h"
#include <iostream>

GeometricObject::GeometricObject(void): 
    material_ptr(NULL) {}
GeometricObject::GeometricObject(Material* material_ptr): 
    material_ptr(material_ptr) {}
GeometricObject::GeometricObject(const GeometricObject& object){
    if (object.material_ptr){
        material_ptr = object.material_ptr->clone();
    } else {
        material_ptr = NULL;
    }
}
GeometricObject& GeometricObject::operator=(const GeometricObject& rhs){
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

Material* GeometricObject::get_material(void) const{
    return material_ptr;
}

void GeometricObject::set_material(Material* material_ptr){
    this->material_ptr = material_ptr;
}

BBox get_bounding_box(void){
    return BBox();
}
void add_object(GeometricObject* object_ptr){}

Vector3D
GeometricObject::get_normal(void) const {
    return (Vector3D(0, 1, 0));
}