#ifndef __SHADE_REC__
#define __SHADE_REC__

// For NULL in the .cpp file????
#include <vector>

class World;
class Material;

#include "Vector3D.h"
#include "Ray.h"

class ShadeRec{
    public:
        // Did it hit an object? If so, what material, world coor, texture coors?, normal at hit point, ray for specular stuff, recursion depth, ray parameter, world reference, 
        bool hitAnObject;
        Material* material_ptr;
        Vector3D hitPoint;
        Vector3D localHitPoint;
        Vector3D normal;
        Ray ray;
        int depth;
        double t;
        World& world;

        ShadeRec(World& world);
        ShadeRec(const ShadeRec& sr);
        ~ShadeRec(void);
};

#endif