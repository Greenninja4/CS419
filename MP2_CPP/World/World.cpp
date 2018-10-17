#include "World.h"

#include <iostream>
#include <fstream>

#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Constants.h"
#include "Ambient.h"
#include "Directional.h"
#include "Point.h"
#include "Matte.h"
#include "RayCast.h"
#include "Material.h"
#include "GeometricObject.h"

using namespace std;

// Constructor & destructor
World::World(void):
    backgroundColor(BLACK), 
    tracer_ptr(NULL), 
    ambient_ptr(new Ambient)
    {}
World::~World (void){
    delete_tracer();
    delete_objects();
    delete_ambient();
    delete_lights();
}
void World::delete_tracer(void){
    if(tracer_ptr){
        delete tracer_ptr;
        tracer_ptr = NULL;
    }
}
void World::delete_objects(void) {
	for (int j = 0; j < objects.size(); j++) {
        if(objects[j]){
            delete objects[j];
            objects[j] = NULL;
        }
	}
	objects.erase(objects.begin(), objects.end());
}
void World::delete_ambient(){
    if (ambient_ptr){
        delete ambient_ptr;
        ambient_ptr = NULL;
    }
}
void World::delete_lights(){
    for (int j = 0; j < lights.size(); j++) {
        if(lights[j]){
            delete lights[j];
            lights[j] = NULL;
        }
	}
    lights.erase(lights.begin(), lights.end());
}

void World::build(void){
    // View Plane
    vp = ViewPlane(400, 400, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(1, WHITE);

    // Background color
    backgroundColor = BLACK;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Directional* directional_ptr = new Directional(1.0, WHITE, Vector3D(1, 1, 2));
    add_light(directional_ptr);

    // Materials
    Matte* matte_ptr = new Matte(0.25, 0.75, BLUE);
    Matte* matte_ptr2 = new Matte(0.25, 0.75, YELLOW);

    // Objects
    Sphere* sphere_ptr = new Sphere(Vector3D(-45, 45, 40), 50);
    sphere_ptr->set_material(matte_ptr);
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(45, 45, 20), 30);
    sphere_ptr->set_material(matte_ptr);
    addObject(sphere_ptr);

    Triangle* triangle_ptr = new Triangle(Vector3D(-110, -85, 80), Vector3D(120, 10, 20), Vector3D(-40, 50, -30));
    triangle_ptr->set_material(matte_ptr2);
    addObject(triangle_ptr);

    triangle_ptr = new Triangle(Vector3D(-10, -8, 80), Vector3D(-120, 130, 20), Vector3D(-400, 50, 30));
    triangle_ptr->set_material(matte_ptr2);
    addObject(triangle_ptr);
}

void World::addObject(GeometricObject* object_ptr){
    objects.push_back(object_ptr);
}

void World::add_light(Light* light){
    lights.push_back(light);
}

void World::set_ambient_light(Light* light_ptr){
    ambient_ptr = light_ptr;
}

ShadeRec World::hitObjects(const Ray& ray){
    ShadeRec sr(*this);
    double t;
    Vector3D normal;
    Vector3D localHitPoint;
    double tmin = KHUGEVALUE;
    int numObjects = objects.size();

    for (int j = 0; j < numObjects; j++){
        if ( objects[j]->hit(ray, t, sr) && (t < tmin) ){
            sr.hitAnObject = true;
            tmin = t;
            sr.material_ptr = objects[j]->get_material();
            sr.hitPoint = ray.o + t * ray.d;
            normal = sr.normal;
            localHitPoint = sr.localHitPoint;
        }
    }
    if (sr.hitAnObject){
        sr.t = tmin;
        sr.normal = normal;
        sr.localHitPoint = localHitPoint;
    }
    
    return sr;
}

void World::renderScene(void) const{
    Vector3D color;
    Ray ray;
    double zw = 100.0;
    double x, y;

    ofstream file;
    file.open("image.ppm");
    file << "P3 " << vp.vres << " " << vp.hres << " " << 255 << "\n";

    ray.d = Vector3D(0, 0, -1);

    for (int r = 0; r < vp.vres; r++){
        for (int c = 0; c < vp.hres; c++){
            x = vp.pixelSize * (c - 0.5 * (vp.hres - 1.0));
            y = vp.pixelSize * (r - 0.5 * (vp.vres - 1.0));
            ray.o = Vector3D(x, y, zw);
            color = tracer_ptr->traceRay(ray);
            file << (int) color.x << " " << (int) color.y << " " << (int) color.z << "\t";
        }
        file << "\n";
    }

    file.close();
}

