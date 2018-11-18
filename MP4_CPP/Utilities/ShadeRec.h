#ifndef __SHADE_REC__
#define __SHADE_REC__

class Material;
class World;

#include "Vector3D.h"
#include "Ray.h"

class ShadeRec{
    public:
        // Variables
        bool hit_an_object;         // Was an object hit?
        Material* material_ptr;     // What material?
        Vector3D hit_point;         // Where? (World coordinates)
        Vector3D local_hit_point;   // Where for texture? (World coordinates)
        Vector3D normal;            // Normal at hit_point
        Ray ray;                    // Ray that hit the object
        int depth;                  // Recursion depth
        double t;                   // Ray parameter
        double u, v;                // Texture coordinates
        const World& world;         // World

        // Big 6
        ShadeRec(const World& world);
        ShadeRec(const ShadeRec& sr);
        ShadeRec& operator= (const ShadeRec& rhs);
        ~ShadeRec(void);
};

#endif