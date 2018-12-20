#ifndef __CONSTANT_COLOR__
#define __CONSTANT_COLOR__

#include "Texture.h"

class ConstantColor : public Texture {
    public: 
        // Variables
        Vector3D color;

        // Big 6
        ConstantColor(void);
        ConstantColor(const Vector3D& color);
        ConstantColor(const ConstantColor& constant_color);
        virtual Texture* clone(void) const;
        virtual ~ConstantColor(void);
        ConstantColor& operator= (const ConstantColor& rhs);

        // Functions
        virtual Vector3D get_color(const ShadeRec& sr) const;
};

#endif