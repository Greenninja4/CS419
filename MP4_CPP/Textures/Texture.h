#ifndef __TEXTURE__
#define __TEXTURE__

#include "ShadeRec.h"
#include "Vector3D.h"

class Texture {
    public: 
        // Big 6
        Texture(void);
        Texture(const Texture& texture);
        virtual Texture* clone(void) const = 0;
        virtual ~Texture(void);
        Texture& operator= (const Texture& rhs);

        // Functions
        virtual Vector3D get_color(const ShadeRec& sr) const = 0;
};

#endif