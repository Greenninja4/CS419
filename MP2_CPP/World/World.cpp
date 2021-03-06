#include "World.h"
// Utilities
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Constants.h"
#include "Ray.h"
#include "ShadeRec.h"
// Tracers
#include "RayCast.h"
// GeometricObjects
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleMesh.h"
// Cameras
#include "Pinhole.h"
#include "Orthographic.h"
// Materials
#include "Matte.h"
#include "Phong.h"
// Lights
#include "Ambient.h"
#include "Directional.h"
#include "Point.h"
// Acceleration
#include "BVH.h"

using namespace std;

// Big 6
World::World(void):
    vp(), 
    background_color(BLACK), 
    tracer_ptr(NULL), 
    ambient_ptr(new Ambient), 
    camera_ptr(NULL) {}
World::~World(void){
    delete_tracer();
    delete_ambient();
    delete_lights();
    delete_objects();
    delete_camera();
}
void World::delete_tracer(void){
    if(tracer_ptr){
        delete tracer_ptr;
        tracer_ptr = NULL;
    }
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
void World::delete_objects(void) {
	for (int j = 0; j < objects.size(); j++) {
        if(objects[j]){
            delete objects[j];
            objects[j] = NULL;
        }
	}
	objects.erase(objects.begin(), objects.end());
}
void World::delete_camera(void){
    if(camera_ptr){
        delete camera_ptr;
        camera_ptr = NULL;
    }
}

// Getters & Setters
void World::set_tracer_ptr(Tracer* tracer_ptr){
    this->tracer_ptr = tracer_ptr;
}
const Tracer* World::get_tracer_ptr(void) const{
    return tracer_ptr;
}
void World::set_ambient(Light* ambient_ptr){
    this->ambient_ptr = ambient_ptr;
}
const Light* World::get_ambient(void) const{
    return ambient_ptr;
}
void World::set_camera(Camera* camera_ptr){
    this->camera_ptr = camera_ptr;
}
// const Camera* World::get_camera(void) const{
//     return camera_ptr;
// }
const vector<Light*>& World::get_lights(void) const{
    return lights;
}
// const vector<GeometricObject*>& World::get_objects(void) const{
//     return objects;
// }
void World::set_background_color(const Vector3D& color){
    this->background_color = color;
}
Vector3D World::get_background_color(void) const{
    return background_color;
}
void World::set_vp(const ViewPlane& vp){
    this->vp = vp;
}
ViewPlane World::get_vp(void) const{
    return vp;
}

// Functions
void World::add_object(GeometricObject* object_ptr){
    objects.push_back(object_ptr);
}
void World::add_light(Light* light){
    lights.push_back(light);
}
void World::render_scene(void) const{
    if (camera_ptr){
        camera_ptr->render_scene(*this);
        return;
    }
    Vector3D color;
    Ray ray;
    ray.d = Vector3D(0, 0, -1);
    double x, y, zw = 100.0;

    ofstream file;
    file.open("image.ppm");
    file << "P3 " << vp.hres << " " << vp.vres << " " << 255.0 << "\n";

    for (int r = 0; r < vp.vres; r++){
        for (int c = 0; c < vp.hres; c++){
            x = vp.pixelSize * (c - 0.5 * (vp.hres - 1.0));
            y = vp.pixelSize * (r - 0.5 * (vp.vres - 1.0));
            ray.o = Vector3D(x, y, zw);
            color = max_to_one(tracer_ptr->trace_ray(ray));
            color = color * 255;
            file << (int) color.x << " " << (int) color.y << " " << (int) color.z << "\t";
        }
        file << "\n";
    }
    file.close();
}
Vector3D World::max_to_one(const Vector3D& c) const{
    double max_value = MAX(c.x, MAX(c.y, c.z));
    if (max_value > 1.0){
        return (c / max_value);
    } else {
        return c;
    }
}
Vector3D World::clamp_to_color(const Vector3D& color) const{
    Vector3D c(color);
    if (c.x > 1.0 || c.y > 1.0 || c.z > 1.0){
        c = RED;
    }
    return c;
}
ShadeRec World::hit_objects(const Ray& ray){
    ShadeRec sr(*this);
    double t;
    Vector3D normal;
    Vector3D local_hit_point;
    double tmin = KHUGEVALUE;
    int numObjects = objects.size();

    for (int j = 0; j < numObjects; j++){
        if ( objects[j]->hit(ray, t, sr) && (t < tmin) ){
            sr.hit_an_object = true;
            tmin = t;
            sr.material_ptr = objects[j]->get_material();
            sr.hit_point = ray.o + t * ray.d;
            normal = sr.normal;
            local_hit_point = sr.local_hit_point;
        }
    }
    if (sr.hit_an_object){
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }
    return sr;
}














void World::orthographic_build(void){
        float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(100, 100, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(-12, 15, 30));
    add_light(point_ptr);

    // Camera: Orthographic
    Orthographic* orthographic_ptr = new Orthographic();
    Vector3D eye1(10, 0, 0);
    orthographic_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    orthographic_ptr->lookat = lookat1;
    float distance1 = 100;
    orthographic_ptr->distance = distance1;
    orthographic_ptr->compute_uvw();
    this->camera_ptr = orthographic_ptr;

    // Materials
    //Matte* emissive_ptr = new Matte(1, 1, WHITE);
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Matte* red_ptr = new Matte(0.25, 0.75, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);
    //Phong* yellow_phong_ptr = new Phong(0.25, 0.75, 0.5, 3.0, YELLOW);

    // Light
    Sphere* sphere_ptr = new Sphere(Vector3D(-12, 15, 30), 0.1);
    // sphere_ptr->set_material(emissive_ptr);
    // add_object(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0.0, 2.4, 0), 1.5);
    sphere_ptr->set_material(red_ptr);
    add_object(sphere_ptr);

    Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    tri_ptr->set_material(green_ptr);
    add_object(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    add_object(plane_ptr);

}

void World::shadow_build(void){
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(100, 100, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(-12, 15, 30));
    add_light(point_ptr);

    // Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    pinhole_ptr->zoom = 0.5;
    Vector3D eye1(2, 2.5, 15);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0 + 3, 2.5, 0);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 700;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    this->camera_ptr = pinhole_ptr;

    // Materials
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Matte* red_ptr = new Matte(0.25, 0.75, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);

    // Objects
    Sphere* sphere_ptr = new Sphere(Vector3D(0.0, 2.4, 0), 1.5);
    sphere_ptr->set_material(red_ptr);
    add_object(sphere_ptr);

    Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    tri_ptr->set_material(green_ptr);
    add_object(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    add_object(plane_ptr);
}



void World::cow_mesh_build(void){
    float light_radiance = 4.0; 

    // View Plane
    vp = ViewPlane(100, 100, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(1, 1, 1));
    add_light(point_ptr);

    // Camera
    // // Camera: Pinhole
    // Pinhole* pinhole_ptr = new Pinhole();
    // pinhole_ptr->up = Vector3D(0, -1, 0);
    // Vector3D eye1(0, -10, 0);
    // pinhole_ptr->eye = eye1;
    // Vector3D lookat1(0, 0, 0);
    // pinhole_ptr->lookat = lookat1;
    // float distance1 = 100;
    // pinhole_ptr->distance = distance1;
    // pinhole_ptr->compute_uvw();
    // pinhole_ptr->zoom = 1;
    // this->camera_ptr = pinhole_ptr;

    // Camera: Orthographic
    Orthographic* orthographic_ptr = new Orthographic();
    Vector3D eye1(0, 0, 2);
    orthographic_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    orthographic_ptr->lookat = lookat1;
    float distance1 = 2;
    orthographic_ptr->distance = distance1;
    orthographic_ptr->up = Vector3D(0, -1, 0);
    orthographic_ptr->zoom = 15;
    orthographic_ptr->compute_uvw();
    this->camera_ptr = orthographic_ptr;

    //Phong* yellow_phong_ptr = new Phong(0.25, 0.75, 0.5, 3.0, YELLOW);

    // Sphere *sphere_ptr = new Sphere(Vector3D(0, 0, 0), 1.0);
    // Matte* red_ptr = new Matte(0.25, 0.75, RED);
    // sphere_ptr->set_material(red_ptr);
    // add_object(sphere_ptr);

    // Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    // tri_ptr->set_material(green_ptr);
    // add_object(tri_ptr);

    // Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    // plane_ptr->set_material(yellow_ptr);
    // add_object(plane_ptr);

    TriangleMesh* mesh = new TriangleMesh("cow.obj");
    Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);
    mesh->set_material(blue_ptr);
    add_object(mesh);
}



void World::cow_mesh_bvh_build(void){
    float light_radiance = 4.0; 

    // View Plane
    vp = ViewPlane(100, 100, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(1, 1, 1));
    add_light(point_ptr);

    // Camera
    // // Camera: Pinhole
    // Pinhole* pinhole_ptr = new Pinhole();
    // pinhole_ptr->up = Vector3D(0, -1, 0);
    // Vector3D eye1(0, -10, 0);
    // pinhole_ptr->eye = eye1;
    // Vector3D lookat1(0, 0, 0);
    // pinhole_ptr->lookat = lookat1;
    // float distance1 = 100;
    // pinhole_ptr->distance = distance1;
    // pinhole_ptr->compute_uvw();
    // pinhole_ptr->zoom = 1;
    // this->camera_ptr = pinhole_ptr;

    // Camera: Orthographic
    Orthographic* orthographic_ptr = new Orthographic();
    Vector3D eye1(0, 0, 2);
    orthographic_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    orthographic_ptr->lookat = lookat1;
    float distance1 = 2;
    orthographic_ptr->distance = distance1;
    orthographic_ptr->up = Vector3D(0, -1, 0);
    orthographic_ptr->zoom = 15;
    orthographic_ptr->compute_uvw();
    this->camera_ptr = orthographic_ptr;

    //Phong* yellow_phong_ptr = new Phong(0.25, 0.75, 0.5, 3.0, YELLOW);

    // Sphere *sphere_ptr = new Sphere(Vector3D(0, 0, 0), 1.0);
    // Matte* red_ptr = new Matte(0.25, 0.75, RED);
    // sphere_ptr->set_material(red_ptr);
    // add_object(sphere_ptr);

    vector<GeometricObject*> bvh_objects;

    Triangle* tri_ptr = new Triangle(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 10.0, 0.0), Vector3D(10, 10, 0));
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);
    tri_ptr->set_material(green_ptr);
    bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, 0.0, 0), Vector3D(0.1, 0.1, 0));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    TriangleMesh* mesh = new TriangleMesh("cow.obj");
    Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);
    mesh->set_material(blue_ptr);
    bvh_objects.push_back(mesh);


    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}


void World::shadow_bvh_build(void){
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(1000, 1000, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(12, 15, 30));
    add_light(point_ptr);

    // Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    pinhole_ptr->zoom = 0.5;
    Vector3D eye1(2, 2.5, 15);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0 + 3, 2.5, 0);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 700;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    this->camera_ptr = pinhole_ptr;

    // Materials
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Matte* red_ptr = new Matte(0.25, 0.75, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);

    // BVH Objects
    vector<GeometricObject*> bvh_objects;

    // Objects
    Sphere* sphere_ptr = new Sphere(Vector3D(0.0, 2.4, 0), 1.5);
    sphere_ptr->set_material(red_ptr);
    bvh_objects.push_back(sphere_ptr);

    Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    tri_ptr->set_material(green_ptr);
    bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}


void World::spheres_bvh_build(void){
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(1000, 1000, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(12, 15, 30));
    add_light(point_ptr);

    // Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    pinhole_ptr->zoom = 0.5;
    Vector3D eye1(2, 2.5, 15);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0 + 3, 2.5, 0);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 700;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    this->camera_ptr = pinhole_ptr;

    // Materials
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Phong* red_ptr = new Phong(0.25, 0.75, 0.5, 3.0, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);

    // BVH Objects
    vector<GeometricObject*> bvh_objects;

    // Objects

    int num = 22;
    for (int i = 0; i < num; i++){
        for (int j = 0; j < num; j++){
            for (int k = 0; k < num; k++){
                Sphere* sphere_ptr = new Sphere(Vector3D(i, j, -2*k), 0.3);
                sphere_ptr->set_material(red_ptr->clone());
                bvh_objects.push_back(sphere_ptr);
            }
        }
    }

    Sphere* sphere_ptr = new Sphere(Vector3D(0.0, 2.4, 0), 1.5);
    sphere_ptr->set_material(red_ptr);
    bvh_objects.push_back(sphere_ptr);

    Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    tri_ptr->set_material(green_ptr);
    bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}


void World::cow_mesh_bvh_2_build(void){
    float light_radiance = 4.0; 

    // View Plane
    vp = ViewPlane(300, 300, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(1, 1, 1));
    add_light(point_ptr);

    //Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(0, 0, 2);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 100;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = 1;
    this->camera_ptr = pinhole_ptr;

    // // Camera: Orthographic
    // Orthographic* orthographic_ptr = new Orthographic();
    // Vector3D eye1(0, 0, 2);
    // orthographic_ptr->eye = eye1;
    // Vector3D lookat1(0, 0, -1);
    // orthographic_ptr->lookat = lookat1;
    // float distance1 = 2;
    // orthographic_ptr->distance = distance1;
    // orthographic_ptr->up = Vector3D(0, -1, 0);
    // orthographic_ptr->zoom = 15;
    // orthographic_ptr->compute_uvw();
    // this->camera_ptr = orthographic_ptr;

    //Phong* yellow_phong_ptr = new Phong(0.25, 0.75, 0.5, 3.0, YELLOW);

    // Sphere *sphere_ptr = new Sphere(Vector3D(0, 0, 0), 1.0);
    // Matte* red_ptr = new Matte(0.25, 0.75, RED);
    // sphere_ptr->set_material(red_ptr);
    // add_object(sphere_ptr);

    vector<GeometricObject*> bvh_objects;

    // Triangle* tri_ptr = new Triangle(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 10.0, 0.0), Vector3D(10, 10, 0));
    // Matte* green_ptr = new Matte(0.25, 0.75, GREEN);
    // tri_ptr->set_material(green_ptr);
    // bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.02));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    TriangleMesh* mesh = new TriangleMesh("cow.obj");
    //Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);
    Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh->set_material(blue_ptr);
    bvh_objects.push_back(mesh);


    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::cow_mesh_no_bvh_2_build(void){
    float light_radiance = 4.0; 

    // View Plane
    vp = ViewPlane(300, 300, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(1, 1, 1));
    add_light(point_ptr);

    //Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(0, 0, 2);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 100;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = 1;
    this->camera_ptr = pinhole_ptr;

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.02));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    add_object(plane_ptr);

    TriangleMesh* mesh = new TriangleMesh("cow.obj");
    //Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);
    Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh->set_material(blue_ptr);
    add_object(mesh);
}


void World::build(void){
    //cow_mesh_bvh_build();
    //shadow_bvh_build();
    //spheres_bvh_build();
    //cow_mesh_bvh_2_build();
    //cow_mesh_no_bvh_2_build();
    spheres_bvh_build();
}