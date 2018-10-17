#ifndef __WORLD__
#define __WORLD__

#include <vector>

#include "ViewPlane.h"
#include "Vector3D.h"
#include "Tracer.h"
#include "GeometricObject.h"
#include "ShadeRec.h"
#include "Ray.h" 
#include "Light.h"

using namespace std;

class World{
    public:
        // View plane, background color, tracer, ambient light, other lights, objects
        ViewPlane vp;
        Vector3D backgroundColor;
        Tracer* tracer_ptr;
        Light* ambient_ptr;
        vector<Light*> lights;
        vector<GeometricObject*> objects;

        // Constructor, destructor, deletes called in destructor
        World(void);
        ~World (void);
        void delete_tracer(void);
        void delete_objects(void);
        void delete_ambient(void);
        void delete_lights(void);

        // Member functions
        void addObject(GeometricObject* object_ptr);
        void add_light(Light* light_ptr);
        void set_ambient_light(Light* light_ptr);
        void build(void);
        void renderScene(void) const;
        ShadeRec hitObjects(const Ray& ray);

        
};

#endif