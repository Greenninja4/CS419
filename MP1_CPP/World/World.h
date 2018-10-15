#ifndef __WORLD__
#define __WORLD__

#include <vector>

#include "ViewPlane.h"
#include "Vector3D.h"
#include "Tracer.h"
#include "GeometricObject.h"
#include "ShadeRec.h"
#include "Ray.h"

class World{
    public:
        // Class members
        ViewPlane vp;
        Vector3D backgroundColor;
        Tracer* tracer_ptr;
        std::vector<GeometricObject*> objects;

        // Constructor, destructor, deletes called in destructor
        World(void);
        ~World (void);
        void delete_tracer(void);
        void delete_objects(void);

        // Member functions
        void build(void);
        void addObject(GeometricObject* object_ptr);
        ShadeRec hitBareBonesObjects(const Ray& ray);
        void renderScene(void) const;


        
};

#endif