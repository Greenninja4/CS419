#include "Compound.h"
#include "Constants.h"

// Big 6
Compound::Compound(void): 
    GeometricObject() {}
Compound::Compound(const Compound& compound) :
    GeometricObject(compound) {}
Compound& Compound::operator= (const Compound& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    copy_objects(rhs.objects);
    return *this;
}
Compound::~Compound (void){
    delete_objects();
}
Compound* Compound::clone(void) const{
    return (new Compound(*this));
}

// Functions
void Compound::add_object(GeometricObject* object_ptr){
    objects.push_back(object_ptr);
}
int Compound::get_num_object(){
    return objects.size();
}
void Compound::copy_objects(std::vector<GeometricObject*> rhs_objects){
    delete_objects();
    for (int j = 0; j < rhs_objects.size(); j++){
        objects.push_back(rhs_objects[j]->clone());
    }
}
void Compound::delete_objects(void){
    for (int j = 0; j < this->get_num_object(); j++){
        delete objects[j];
        objects[j] = NULL;
    }
    objects.erase(objects.begin(), objects.end());
}
void Compound::set_material(Material* material_ptr){
    for (int j = 0; j < this->get_num_object(); j++){
        objects[j]->set_material(material_ptr);
    }
}
bool Compound::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    double t;
    Vector3D normal;
    Vector3D local_hit_point;
    bool hit = false;
    tmin = KHUGEVALUE;

    for (int j = 0; j < objects.size(); j++){
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            hit	= true;
            tmin = t;
            material_ptr = objects[j]->get_material();
            normal = sr.normal;
            local_hit_point	= sr.local_hit_point;
        }
    }

    if (hit) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return hit;
}
bool Compound::shadow_hit(const Ray& ray, double& tmin) const{
    double t;
    Vector3D normal;
    Vector3D local_hit_point;
    bool hit = false;
    tmin = KHUGEVALUE;

    for (int j = 0; j < objects.size(); j++){
        if (objects[j]->shadow_hit(ray, t) && (t < tmin)) {
            hit	= true;
            tmin = t;
        }
    }
    return hit;
}

BBox Compound::get_bounding_box(void){
    return bbox;
}
void Compound::set_bounding_box(void){
    bbox = BBox();
}