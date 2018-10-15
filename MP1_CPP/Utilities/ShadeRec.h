#ifndef __SHADE_REC__
#define __SHADE_REC__

class World;
#include "Vector3D.h"

class ShadeRec{
    public:
        // Did it hit an object? If so, where, normal, color. World obj.
        bool hitAnObject;
        Vector3D localHitPoint;
        Vector3D normal;
        Vector3D color;
        World& world;

        ShadeRec(World& world);
        ShadeRec(const ShadeRec& sr);
        ShadeRec& operator=(const ShadeRec& rhs);
        ~ShadeRec(void);
};

#endif