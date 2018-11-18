#include "BVH.h"
#include <algorithm>

using namespace std;

// Big 6
BVH::BVH(void): 
    bbox(), 
    left(NULL), 
    right(NULL) {}
BVH::BVH(const BVH& bvh): 
    objects(bvh.objects), 
    bbox(bvh.bbox), 
    left(bvh.left), 
    right(bvh.right) {}
BVH& BVH::operator=(const BVH& rhs){
    if (this == &rhs){
        return *this;
    }
    objects = rhs.objects;
    bbox = rhs.bbox;
    left = rhs.left;
    right = rhs.right;
    return *this;
}
BVH::~BVH(void){
    delete_objects();
}
BVH* BVH::clone(void) const{
    return (new BVH(*this));
}

// IMPORTANT BVH CONSTRUCTORS
// Base Case
BVH::BVH(GeometricObject* left, GeometricObject* right){
    this->left = left;
    this->right = right;
    vector<GeometricObject*> objs;
    objs.push_back(left);
    objs.push_back(right);
    bbox = surround(objs);
    // cout << "BBOX: " << bbox.p0.x << ", " << bbox.p0.y << ", " << bbox.p0.z << ", ";
    // cout <<             bbox.p1.x << ", " << bbox.p1.y << ", " << bbox.p1.z << endl;
}
// Recursive Case
BVH::BVH(const vector<GeometricObject*>& objects, int num_objects, int axis){
    // 0, 1, or 2 objects
    if (objects.size() == 0){
        *this = BVH();
        return;
    }
    if (objects.size() == 1){
        *this = BVH(objects[0], objects[0]);
        return;
    }
    if (objects.size() == 2){
        *this = BVH(objects[0], objects[1]);
        return;
    }
    
    // Get overall bbox
    bbox = surround(objects);

    // Get current axis pivot point
    Vector3D pivot = (bbox.p0 + bbox.p1) / 2.0;
    double pivot_point = pivot.x;
    if (axis == 1){
        pivot_point = pivot.y;
    } else if (axis == 2){
        pivot_point = pivot.z;
    }

    // Get current mid_point index
    int mid_point = qsplit(objects, objects.size(), pivot_point, axis);
    

    vector<GeometricObject*>::const_iterator left_start = objects.begin();
    vector<GeometricObject*>::const_iterator left_end = objects.begin() + mid_point;
    vector<GeometricObject*>::const_iterator right_start = objects.begin() + mid_point;
    vector<GeometricObject*>::const_iterator right_end = objects.end();
    vector<GeometricObject*> left_objs(left_start, left_end);
    vector<GeometricObject*> right_objs(right_start, right_end);

    int next_axis = (axis + 1) % 3;
    left = new BVH(left_objs, mid_point, next_axis);
    right = new BVH(right_objs, objects.size() - mid_point, next_axis);
}

// Functions
void BVH::add_object(GeometricObject* object_ptr){
    objects.push_back(object_ptr);
}
void BVH::delete_objects(void){
    for (int i = 0; i < objects.size(); i++){
        if (objects[i]){
            delete objects[i];
        }
    }
    objects.erase(objects.begin(), objects.end());
}
bool BVH::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    // No hit
    if ( !(bbox.hit(ray, tmin, sr)) ){
        return false;
    }
    
    // Left hit
    bool left_hit = false;
    double left_tmin = tmin;
    ShadeRec left_sr(sr);
    if (left->get_bounding_box().hit(ray, left_tmin, left_sr)){
        left_hit = left->hit(ray, left_tmin, left_sr);
    }
    // Right hit
    bool right_hit = false;
    double right_tmin = tmin;
    ShadeRec right_sr(sr);
    if (right->get_bounding_box().hit(ray, right_tmin, right_sr)){
        right_hit = right->hit(ray, right_tmin, right_sr);
    }

    // Which hit is closer?
    // Only left hit
    if (left_hit && !right_hit){
        tmin = left_tmin;
        sr = left_sr;
        material_ptr = left->material_ptr;
    }
    // Only right hit
    if (!left_hit && right_hit){
        tmin = right_tmin;
        sr = right_sr;
        material_ptr = right->material_ptr;
    }
    // Hit both
    if (left_hit && right_hit){
        if (right_tmin < left_tmin){
            tmin = right_tmin;
            sr = right_sr;
            material_ptr = right->material_ptr;
        } else {
            tmin = left_tmin;
            sr = left_sr;
            material_ptr = left->material_ptr;
        }
    }

    // Return
    return (left_hit || right_hit);
}
bool BVH::shadow_hit(const Ray& ray, double& tmin) const{
    if ( !(bbox.shadow_hit(ray, tmin))){
        return false;
    }
    bool left_hit = left->shadow_hit(ray, tmin);
    bool right_hit = right->shadow_hit(ray, tmin);
    return (left_hit || right_hit);
}
BBox BVH::get_bounding_box(void){
    return bbox;
}
void BVH::set_bounding_box(void){
    bbox = surround(objects);
}



// Non-member functions

// Surrounding BBox
BBox surround(const vector<GeometricObject*>& objects){
    BBox bbox = BBox(Vector3D(-1, 1, -1), Vector3D(1, -1, 1));
    // No objects
    if (objects.size() == 0){
        return bbox;
    }
    // 1+ Objects
    Vector3D v0 = Vector3D(objects[0]->get_bounding_box().p0);
    Vector3D v1 = Vector3D(objects[0]->get_bounding_box().p1);
    for (int i = 1; i < objects.size(); i++){
        v0.x = MIN(v0.x, objects[i]->get_bounding_box().p0.x);
        v0.y = MIN(v0.y, objects[i]->get_bounding_box().p0.y);
        v0.z = MIN(v0.z, objects[i]->get_bounding_box().p0.z);
        v1.x = MAX(v1.x, objects[i]->get_bounding_box().p1.x);
        v1.y = MAX(v1.y, objects[i]->get_bounding_box().p1.y);
        v1.z = MAX(v1.z, objects[i]->get_bounding_box().p1.z);
    }
    bbox.p0 = v0;
    bbox.p1 = v1;
    return bbox;
}

// BBox center-point at axis compare functions
bool compare0(GeometricObject* obj0, GeometricObject* obj1){
    double x0, x1;
    x0 = obj0->get_bounding_box().p1.x - obj0->get_bounding_box().p0.x;
    x1 = obj1->get_bounding_box().p1.x - obj1->get_bounding_box().p0.x;
    return (x0 < x1);
}
bool compare1(GeometricObject* obj0, GeometricObject* obj1){
    double y0, y1;
    y0 = obj0->get_bounding_box().p1.y - obj0->get_bounding_box().p0.y;
    y1 = obj1->get_bounding_box().p1.y - obj1->get_bounding_box().p0.y;
    return (y0 < y1);
}
bool compare2(GeometricObject* obj0, GeometricObject* obj1){
    double z0, z1;
    z0 = obj0->get_bounding_box().p1.z - obj0->get_bounding_box().p0.z;
    z1 = obj1->get_bounding_box().p1.z - obj1->get_bounding_box().p0.z;
    return (z0 < z1);
}

// BBOX SORTING ALGORITHM
int qsplit(vector<GeometricObject*> objects, int num_objects, double pivot_point, int axis){
    // X-axis split
    if (axis == 0){
        sort(objects.begin(), objects.end(), compare0);
        // int mid_point = 0;
        // for (int i = 0; i < objects.size(); i++){
        //     double x0 = objects[i]->get_bounding_box().p1.x - objects[i]->get_bounding_box().p0.x;
        //     if (x0 > pivot_point){
        //         return mid_point;
        //     }
        //     mid_point++;
        // }

    // Y-axis split
    } else if (axis == 1){
        sort(objects.begin(), objects.end(), compare1);
        // int mid_point = 0;
        // for (int i = 0; i < objects.size(); i++){
        //     double y0 = objects[i]->get_bounding_box().p1.y - objects[i]->get_bounding_box().p0.y;
        //     if (y0 > pivot_point){
        //         return mid_point;
        //     }
        //     mid_point++;
        // }

    // Z-axis split
    } else if (axis == 2){
        sort(objects.begin(), objects.end(), compare2);
        // int mid_point = 0;
        // for (int i = 0; i < objects.size(); i++){
        //     double z0 = objects[i]->get_bounding_box().p1.z - objects[i]->get_bounding_box().p0.z;
        //     if (z0 > pivot_point){
        //         return mid_point;
        //     }
        //     mid_point++;
        // }
    }
    return objects.size() / 2;
}