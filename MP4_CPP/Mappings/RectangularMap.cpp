#include "RectangularMap.h"

// Big 6
RectangularMap::RectangularMap(void){}
RectangularMap::RectangularMap(const RectangularMap& rm){}
RectangularMap& RectangularMap::operator= (const RectangularMap& rhs){
    if (this == &rhs){
        return *this;
    }
    return *this;
}

Mapping* RectangularMap::clone(void) const{
    return (new RectangularMap(*this));
}
RectangularMap::~RectangularMap(void){}

// Functions
void RectangularMap::get_texel_coordinates(const Vector3D& hit_point, const int xres, const int yres, int& row, int& col) const{

    // UV coordinates
    float u = (hit_point.z + 1)/2;
    float v = (hit_point.x + 1)/2;

    // Map u and v to texel coordinates
    col = (int) ((xres - 1) * u);
    row = (int) ((yres - 1) * v);
}