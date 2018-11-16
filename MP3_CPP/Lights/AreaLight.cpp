#include "AreaLight.h"

// Big 6
AreaLight::AreaLight(void):
    Light(),
    object_ptr(NULL),
    material_ptr(NULL) {}
AreaLight::AreaLight(GeometricObject* object_ptr):
    Light(),
    object_ptr(object_ptr),
    material_ptr(object_ptr->material_ptr) {}
AreaLight::AreaLight(const AreaLight& area_light):
    Light(area_light) {
    if (area_light.object_ptr){
        object_ptr = area_light.object_ptr->clone();
    } else {
        object_ptr = NULL;
    }
    if (area_light.material_ptr){
        material_ptr = area_light.material_ptr->clone();
    } else {
        material_ptr = NULL;
    }
}
AreaLight::~AreaLight(){
    if (object_ptr){
        delete object_ptr;
        object_ptr = NULL;
    }
    if (material_ptr){
        delete material_ptr;
        material_ptr = NULL;
    }
}
AreaLight& AreaLight::operator= (const AreaLight& rhs){
    if (this == &rhs){
        return *this;
    }
    Light::operator=(rhs);
    if (object_ptr){
        delete object_ptr;
        object_ptr = NULL;
    }
    if (rhs.object_ptr){
        object_ptr = rhs.object_ptr->clone();
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
Light* AreaLight::clone() const{
    return new AreaLight(*this);
}

// Functions
Vector3D AreaLight::get_direction(ShadeRec& sr){
    sample_point = object_ptr->sample();
    light_normal = object_ptr->get_normal(sample_point);
    wi = sample_point - sr.hit_point;
    wi.normalize();
    return wi;
}
Vector3D AreaLight::L(ShadeRec& sr){
    float ndotd = -light_normal * wi;
    if (ndotd > 0.0){
        return material_ptr->get_Le(sr);
    } else {
        return BLACK;
    }
}
bool AreaLight::casts_shadow(void) const{
    return true;
}
bool AreaLight::in_shadow(const Ray& ray, const ShadeRec& sr) const{
    // double t;
    // int num_objects = sr.world.objects.size();
    // float ts = (sample_point - ray.o) * ray.d;
    // for (int j = 0; j < num_objects; j++){
    //     if (sr.world.objects[j]->shadow_hit(ray, t) && t < ts){
    //         return true;
    //     }
    // }
    // return false;

    double t;
    int num_objects = sr.world.objects.size();
    double d = sample_point.distance(ray.o);
    for (int j = 0; j < num_objects; j++){
        if (sr.world.objects[j]->shadow_hit(ray, t) && (t < d)){
            return true;
        }
    }
    return false;
}

void AreaLight::set_object(GeometricObject* object_ptr){
    this->object_ptr = object_ptr;
    this->material_ptr = object_ptr->material_ptr;
}
float AreaLight::G(const ShadeRec& sr) const{
    // cout << light_normal.x << ", " << light_normal.y << ", " << light_normal.z << ", " << endl;
    float ndotd = -light_normal * wi;
    float d_squared = sample_point.distance(sr.hit_point) * sample_point.distance(sr.hit_point);
    return ndotd/d_squared;
}
float AreaLight::pdf(const ShadeRec& sr) const{
    return object_ptr->pdf(sr);
}
