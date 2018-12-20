#include "Texture.h"

// Big 6
Texture::Texture(void){}
Texture::Texture(const Texture& texture){}
Texture::~Texture(void){}
Texture& Texture::operator= (const Texture& rhs){
    if (this == &rhs){
        return *this;
    }
    return *this;
}
