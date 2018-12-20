#include "ConstantColor.h"
#include "Constants.h"

// Big 6
ConstantColor::ConstantColor(void): 
    color(BLACK) {}
ConstantColor::ConstantColor(const Vector3D& color): 
    color(color) {}
ConstantColor::ConstantColor(const ConstantColor& constant_color):
    color(constant_color.color) {}
Texture* ConstantColor::clone(void) const{
    return (new ConstantColor(*this));
}
ConstantColor::~ConstantColor(void){}
ConstantColor& ConstantColor::operator= (const ConstantColor& rhs){
    if (this == &rhs){
        return *this;
    }
    Texture::operator= (rhs);
    color = rhs.color;
    return *this;
}

// Functions
Vector3D ConstantColor::get_color(const ShadeRec& sr) const{
    return color;
}
