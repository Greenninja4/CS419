#include "CSG.h"
#include "Constants.h"
#include "Maths.h"

GeometricObject* left;
GeometricObject* right;
int int_uni_min; // 0 = intersection, 1 = union, 2 = minus
BBox bbox;

// Big 6
CSG::CSG(void): 
    GeometricObject(),
    left(NULL),
    right(NULL),
    int_uni_min(0),
    bbox() {}
CSG::CSG(GeometricObject* left, GeometricObject* right, int int_uni_min):
    GeometricObject(),
    left(left),
    right(right),
    int_uni_min(int_uni_min) {
    set_bounding_box();
}
CSG::CSG(const CSG& csg) :
    GeometricObject(csg),
    left(csg.left),
    right(csg.right),
    int_uni_min(csg.int_uni_min),
    bbox(csg.bbox) {}
CSG& CSG::operator= (const CSG& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    left = rhs.left;
    right = rhs.right;
    int_uni_min = rhs.int_uni_min;
    bbox = rhs.bbox;
    return *this;
}
CSG::~CSG (void){
    if (left){
        delete left;
        left = NULL;
    }
    if (right){
        delete right;
        right = NULL;
    }
}
CSG* CSG::clone(void) const{
    return (new CSG(*this));
}

// Functions
bool CSG::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
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

    // Left min hit
    bool min_left_hit = false;
    double min_left_tmin = tmin;
    ShadeRec min_left_sr(sr);
    if (left->get_bounding_box().hit(ray, min_left_tmin, min_left_sr)){
        min_left_hit = left->min_hit(ray, min_left_tmin, min_left_sr);
    }

    // Right intersect Left
    if (int_uni_min == 0){
        // Are both hit?
        if (left_hit && right_hit){
            if (right_tmin > left_tmin){
                tmin = right_tmin;
                sr = right_sr;
                material_ptr = right->material_ptr;
            } else {
                tmin = left_tmin;
                sr = left_sr;
                material_ptr = left->material_ptr;
            }
        }
        return (left_hit && right_hit);

    // Right U Left
    } else if (int_uni_min == 1){
        // Are both hit?
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
        // Just right hit?
        } else if (right_hit){
            tmin = right_tmin;
            sr = right_sr;
            material_ptr = right->material_ptr;
        // Just left hit?
        } else if (left_hit){
            tmin = left_tmin;
            sr = left_sr;
            material_ptr = left->material_ptr;
        }
        return (left_hit || right_hit);

    // Right - Left
    } else if (int_uni_min == 2){

        // Are both hit?
        if (left_hit && right_hit){
            // Right object is getting a chunk taken out
            if (right_tmin < min_left_tmin){
                // tmin = min_left_tmin;
                // sr = min_left_sr;
                tmin = min_left_tmin;
                sr = min_left_sr;
                material_ptr = left->material_ptr;
            // Right object is not getting a chunk taken out
            } else {
                tmin = right_tmin;
                sr = right_sr;
                material_ptr = right->material_ptr;
            }
            return true;
        // Just right object is hit.
        } else if (right_hit){
            tmin = right_tmin;
            sr = right_sr;
            material_ptr = right->material_ptr;
            return right_hit;
        } else {
            return false;
        }

    // bad parameter, just return left
    } else {
        material_ptr = left->material_ptr;
        return left->hit(ray, tmin, sr);
    }
}
bool CSG::shadow_hit(const Ray& ray, double& tmin) const{
    return false;
}

BBox CSG::get_bounding_box(void){
    return bbox;
}
void CSG::set_bounding_box(void){
    float x0,x1,y0,y1,z0,z1;
    BBox bl = left->get_bounding_box();
    BBox br = right->get_bounding_box();
    x0 = MIN(bl.p0.x, br.p0.x);
    x1 = MAX(bl.p1.x, br.p1.x);
    y0 = MIN(bl.p0.y, br.p0.y);
    y1 = MAX(bl.p1.y, br.p1.y);
    z0 = MIN(bl.p0.z, br.p0.z);
    z1 = MAX(bl.p1.z, br.p1.z);
    bbox = BBox(Vector3D(x0, y0, z0), Vector3D(x1, y1, z1));
}