#include "Checker.h"
#include "Constants.h"

// Big 6
Checker::Checker(void): 
    color1(BLACK),
    color2(WHITE),
    size(1.0) {}
Checker::Checker(const Vector3D& color1, const Vector3D& color2, const float& size): 
    color1(color1),
    color2(color2),
    size(size) {}
Checker::Checker(const Checker& checker):
    color1(checker.color1),
    color2(checker.color2),
    size(checker.size) {}
Texture* Checker::clone(void) const{
    return (new Checker(*this));
}
Checker::~Checker(void){}
Checker& Checker::operator= (const Checker& rhs){
    if (this == &rhs){
        return *this;
    }
    Texture::operator= (rhs);
    color1 = rhs.color1;
    color2 = rhs.color2;
    size = rhs.size;
    return *this;
}

// Functions
Vector3D Checker::get_color(const ShadeRec& sr) const{
    float x = sr.local_hit_point.x;
    float y = sr.local_hit_point.y;
    float z = sr.local_hit_point.z;

    if (((int)(x/size) + (int)(y/size) + (int)(z/size)) % 2 == 0){
        return color1;
    } else {
        return color2;
    }
}
