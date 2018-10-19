#ifndef __MATERIAL__
#define __MATERIAL__

#include "World.h"
#include "Vector3D.h"
#include "ShadeRec.h"

class Material{
    public:
        Material(void);
        Material(const Material& material);
        Material& operator=(const Material& rhs);
        virtual ~Material(void);
        virtual Material* clone(void) const = 0;

        virtual Vector3D shade(ShadeRec& sr);
};

#endif