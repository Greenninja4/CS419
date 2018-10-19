#ifndef __WORLD__
#define __WORLD__

#include <vector>
#include "ViewPlane.h"
#include "Vector3D.h"
#include "Tracer.h"
#include "Light.h"
#include "GeometricObject.h"
#include "Camera.h"

using namespace std;

class World{
    public:
        // Variables
        ViewPlane vp;                       // View Plane
        Vector3D background_color;          // Background Color
        Tracer* tracer_ptr;                 // Tracer Pointer
        Light* ambient_ptr;                 // Ambient Light
        vector<Light*> lights;              // Lights
        vector<GeometricObject*> objects;   // Objects
        Camera* camera_ptr;                 // Camera

        // Big 6
        World(void);
        ~World (void);
        void delete_tracer(void);
        void delete_objects(void);
        void delete_ambient(void);
        void delete_lights(void);
        void delete_camera(void);

        // Getters & Setters
        void set_tracer_ptr(Tracer* tracer_ptr);
        const Tracer* get_tracer_ptr(void) const;
        void set_ambient(Light* ambient_ptr);
        const Light* get_ambient(void) const;
        void set_camera(Camera* camera_ptr);
        // const Camera* World::get_camera(void) const;
        const vector<Light*>& get_lights() const;
        // const vector<GeometricObject*>& World::get_objects(void) const;
        void set_background_color(const Vector3D& color);
        Vector3D get_background_color(void) const;
        void set_vp(const ViewPlane& vp);
        ViewPlane get_vp(void) const;
        
        // Functions
        void add_object(GeometricObject* object_ptr);
        void add_light(Light* light_ptr);
        void render_scene(void) const;
        Vector3D max_to_one(const Vector3D& color) const;
        Vector3D clamp_to_color(const Vector3D& color) const;
        ShadeRec hit_objects(const Ray& ray);
        void build(void);
};

#endif