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
#include "Pinhole.h"
#include "Grid.h"

using namespace std;

// Constructor & destructor
World::World(void):
    backgroundColor(BLACK), 
    tracer_ptr(NULL), 
    ambient_ptr(new Ambient), 
    camera_ptr(NULL)
    {}
World::~World (void){
    delete_tracer();
    delete_objects();
    delete_ambient();
    delete_lights();
    delete_camera();
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
void World::delete_camera(void){
    if(camera_ptr){
        delete camera_ptr;
        camera_ptr = NULL;
    }
}

void World::build(void){
    float light_radiance = 5.0;

    // View Plane
    vp = ViewPlane(600, 400, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    backgroundColor = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 2.0, WHITE, Vector3D(15, 15, 2.5));
    add_light(point_ptr);

    // Camera
    Pinhole* pinhole_ptr = new Pinhole();
    Vector3D eye1(0, -50, 0);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(-1, 3.7, 0);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 800;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    this->camera_ptr = pinhole_ptr;

    // Materials
    Matte* emissive_ptr = new Matte(1, 1, WHITE);
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Matte* red_ptr = new Matte(0.25, 0.75, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);
    Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);

    // Objects
    Sphere* sphere_ptr = new Sphere(Vector3D(15, 15, 2.5), 1.0);
    sphere_ptr->set_material(emissive_ptr);
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(3.85, 2.3, -2.55), 2.3);
    sphere_ptr->set_material(yellow_ptr);
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(-0.7, 1, 4.2), 2);
    sphere_ptr->set_material(red_ptr);
    addObject(sphere_ptr);

    //ORTHO SPHERES
    sphere_ptr = new Sphere(Vector3D(10, 0, 0), 1);
    sphere_ptr->set_material(blue_ptr->clone());
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0, 10, 0), 1);
    sphere_ptr->set_material(blue_ptr);
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0, 0, 10), 1);
    sphere_ptr->set_material(blue_ptr->clone());
    addObject(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0, 0, 0), 1);
    sphere_ptr->set_material(blue_ptr->clone());
    addObject(sphere_ptr);

    // Triangle
    Triangle* tri_ptr = new Triangle(Vector3D(0, 2, 3), Vector3D(2,3,4), Vector3D(3,4,5));
    tri_ptr->set_material(green_ptr);
    addObject(tri_ptr);
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

Vector3D World::max_to_one(const Vector3D& c) const{
    float max_value = max(c.x, max(c.y, c.z));
    if (max_value > 1.0){
        return (c / max_value);
    } else {
        return c;
    }
}

void World::renderScene(void) const{
    if (camera_ptr){
        camera_ptr->render_scene(*this);
        return;
    }


    Vector3D color;
    Ray ray;
    double zw = 100.0;
    double x, y;

    ofstream file;
    file.open("image.ppm");
    file << "P3 " << vp.hres << " " << vp.vres << " " << 255.0 << "\n";

    ray.d = Vector3D(0, 0, -1);

    for (int r = 0; r < vp.vres; r++){
        for (int c = 0; c < vp.hres; c++){
            x = vp.pixelSize * (c - 0.5 * (vp.hres - 1.0));
            y = vp.pixelSize * (r - 0.5 * (vp.vres - 1.0));
            ray.o = Vector3D(x, y, zw);
            color = max_to_one(tracer_ptr->traceRay(ray));
            //file << (int) color.x << " " << (int) color.y << " " << (int) color.z << "\t";
            file << (int) (255 * color.x) << " " << (int) (255 * color.y) << " " << (int) (255 * color.z) << "\t";
        }
        file << "\n";
    }

    file.close();
}

