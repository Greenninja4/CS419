#include "World.h"

#include <iostream>
#include <fstream>

#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Constants.h"
#include "MultipleObjectsTracer.h"

using namespace std;

// Constructor & destructor
World::World(void): 
    vp(), 
    backgroundColor(BLACK),
    tracer_ptr(NULL),
    objects() {}
World::~World (void){
    delete_tracer();
    delete_objects();
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
	//objects.erase(objects.begin(), objects.end());
}

void World::build(void){
    vp = ViewPlane(400, 400, 1.0, 1.0);

    backgroundColor = BLACK;
    tracer_ptr = new MultipleObjectsTracer(this);

    Vector3D p(0.0, 0.0, 0.0);
    Vector3D n(0.0, 1.0, 1.0);
    Vector3D color(0.0, 100.0, 0.0);

    // Plane* plane_ptr = new Plane(p, n, color);
    // addObject(plane_ptr);
    Sphere* sphere_ptr = new Sphere(Vector3D(-45, 45, 40), 50, YELLOW);
    addObject(sphere_ptr);
    sphere_ptr = new Sphere(Vector3D(45, 45, 20), 30, RED);
    addObject(sphere_ptr);
    Triangle* triangle_ptr = new Triangle(Vector3D(-110, -85, 80), Vector3D(120, 10, 20), Vector3D(-40, 50, -30), BLUE);
    addObject(triangle_ptr);
    triangle_ptr = new Triangle(Vector3D(-10, -8, 80), Vector3D(-120, 130, 20), Vector3D(-400, 50, 30), GREEN);
    addObject(triangle_ptr);
}

void World::addObject(GeometricObject* object_ptr){
    objects.push_back(object_ptr);
}

ShadeRec World::hitBareBonesObjects(const Ray& ray){
    ShadeRec sr(*this);
    double t;
    double tmin = KHUGEVALUE;

    for (int i = 0; i < objects.size(); i++){
        if (objects[i]->hit(ray, t, sr) && (t < tmin)){
            sr.hitAnObject = true;
            tmin = t;
            sr.color = objects[i]->color;
        }
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

