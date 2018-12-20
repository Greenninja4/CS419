#ifndef __CHECKER__
#define __CHECKER__

#include "Texture.h"

class Checker : public Texture {
    public: 
        // Variables
        Vector3D color1;
        Vector3D color2;
        float size;

        // Big 6
        Checker(void);
        Checker(const Vector3D& color1, const Vector3D& color2, const float& size);
        Checker(const Checker& checker);
        virtual Texture* clone(void) const;
        virtual ~Checker(void);
        Checker& operator= (const Checker& rhs);

        // Functions
        virtual Vector3D get_color(const ShadeRec& sr) const;
};

#endif