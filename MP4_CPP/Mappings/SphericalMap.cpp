#include "SphericalMap.h"
#include <math.h>

// Big 6
SphericalMap::SphericalMap(void){}
SphericalMap::SphericalMap(const SphericalMap& sm){}
SphericalMap& SphericalMap::operator= (const SphericalMap& rhs){
    if (this == &rhs){
        return *this;
    }
    return *this;
}

Mapping* SphericalMap::clone(void) const{
    return (new SphericalMap(*this));
}
SphericalMap::~SphericalMap(void){}

// Functions
void SphericalMap::get_texel_coordinates(const Vector3D& hit_point, const int xres, const int yres, int& row, int& col) const{
    
    // Spherical coordinates
    float theta = acos(hit_point.y);
    float phi = atan2(hit_point.x, hit_point.z);
    if (phi < 0.0){
        phi += PI * 2;
    }

    // UV coordinates
    float u = phi / (PI * 2);
    float v = 1 - theta / (PI);

    // Map u and v to texel coordinates
    col = (int) ((xres - 1) * u);
    row = (int) ((yres - 1) * v);
}