#include "World.h"
// Utilities
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
#include <vector>
#include "Constants.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Image.h"
// Tracers
#include "RayCast.h"
#include "Whitted.h"
#include "AreaLighting.h"
// GeometricObjects
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleMesh.h"
#include "Rectangle.h"
#include "FastTriMesh.h"
#include "Instance.h"
#include "Deformed.h"
#include "CSG.h"
#include "SmoothMesh.h"
// Cameras
#include "Pinhole.h"
#include "Orthographic.h"
// Materials
#include "Matte.h"
#include "Phong.h"
#include "Reflective.h"
#include "Transparent.h"
#include "Emmissive.h"
#include "SV_Matte.h"
// Lights
#include "Ambient.h"
#include "Directional.h"
#include "Point.h"
#include "AreaLight.h"
// Acceleration
#include "BVH.h"
// Samplers
#include "Jittered.h"
// Mappings
#include "RectangularMap.h"
// Textures
#include "ImageTexture.h"
#include "ConstantColor.h"
#include "Checker.h"
#include "Noise.h"

using namespace std;

// Big 6
World::World(void):
    vp(), 
    background_color(BLACK), 
    tracer_ptr(NULL), 
    ambient_ptr(new Ambient), 
    camera_ptr(NULL),
    num_rays(0) {}
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
        num_rays++;
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
    int size = 600;
    vp = ViewPlane(size, size, 1.0, 1.0);

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
    pinhole_ptr->zoom = 0.3;
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



void World::mirror_build(void){
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(1000, 1000, 1.0, 1.0, 8);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new Whitted(this);

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

    // MIRROR SPHERE
    sphere_ptr = new Sphere(Vector3D(-10.0, 6.0, 3), 4.5);
    Reflective* refl_ptr = new Reflective(0.0, 0.0, 0.0, 1.0, Vector3D(.9, 1, .9), .9);
    sphere_ptr->set_material(refl_ptr);
    bvh_objects.push_back(sphere_ptr);

    // BLUE REFLECTIVE SPHERE
    sphere_ptr = new Sphere(Vector3D(-3.0, 6.0, 1), 1.5);
    refl_ptr = new Reflective(0.1, 0.1, 0.3, 6.0, CYAN*.8, .9);
    sphere_ptr->set_material(refl_ptr);
    bvh_objects.push_back(sphere_ptr);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::transparent_build(void){
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(1000, 1000, 1.0, 1.0, 8);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new Whitted(this);

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
    //Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);

    // BVH Objects
    vector<GeometricObject*> bvh_objects;

    // Objects

    int num = 10;
    for (int i = 0; i < num; i++){
        for (int j = 0; j < num; j++){
            for (int k = 0; k < num; k++){
                Sphere* sphere_ptr = new Sphere(Vector3D(2*i, 2*j - num/2, -2*k), 0.3);
                if (i > num/2){
                    sphere_ptr->set_material(green_ptr->clone());
                } else {
                    sphere_ptr->set_material(red_ptr->clone());
                }
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

    // tri_ptr = new Triangle(Vector3D(-10, 0, 0), Vector3D(10, 0, 0), Vector3D(0, 10, 0));
    // tri_ptr->set_material(blue_ptr);
    // bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    // GLASS/TRANSPARENT SPHERE
    sphere_ptr = new Sphere(Vector3D(2.0, 5, 6), 2);
    Transparent* trans_ptr = new Transparent(0.0, 0.0, 0.2, 2000, BLACK, 0.1, 0.9, .75);
    sphere_ptr->set_material(trans_ptr);
    bvh_objects.push_back(sphere_ptr);

    // GLASS/TRANSPARENT SPHERE 2
    sphere_ptr = new Sphere(Vector3D(8.0, 5, 6), 2);
    trans_ptr = new Transparent(0.0, 0.0, 0.2, 2000, BLACK, 0.1, 0.9, 1.5);
    sphere_ptr->set_material(trans_ptr);
    bvh_objects.push_back(sphere_ptr);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::area_light_build(void){
    // Sampler
    int n = 9;
    int num_samples = n*n;
    int num_sets = 83;
    Sampler* sampler_ptr = new Jittered(num_samples, num_sets);

    // View Plane
    vp = ViewPlane(600, 600, 1.0, 1.0, 8);
    vp.set_sampler(sampler_ptr);

    // Ambient Light
    float light_radiance = 4.0;
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new AreaLighting(this);

    // Lights
    Emmissive* emmissive_ptr = new Emmissive(40, WHITE);

    float width = 4.0;
    float height= 4.0;
    Vector3D center(0.0, 7.0, -7.0);
    Vector3D p0(-0.5 * width, center.y - 0.5 * height, center.z);
    Vector3D a(width, 0.0, 0.0);
    Vector3D b(0.0, height, 0.0);
    
    Rectangle* rectangle_ptr = new Rectangle(p0, a, b, sampler_ptr->clone());
    rectangle_ptr->set_material(emmissive_ptr);
    add_object(rectangle_ptr);

    AreaLight* area_light_ptr = new AreaLight(rectangle_ptr);
    add_light(area_light_ptr);

    // Point Light (for debugging)
    // Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(12, 15, 30));
    // add_light(point_ptr);

    // Camera
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    pinhole_ptr->zoom = 1.0;
    pinhole_ptr->eye = Vector3D(-20, 10, 20);
    pinhole_ptr->lookat = Vector3D(0, 2, 0);
    pinhole_ptr->distance = 1080;
    pinhole_ptr->compute_uvw();
    this->camera_ptr = pinhole_ptr;

    // Materials
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);

    // Objects
    Plane* plane_ptr = new Plane(Vector3D(0, 0, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    add_object(plane_ptr);

    rectangle_ptr = new Rectangle(Vector3D(-3,0,0), Vector3D(2,0,0), Vector3D(0,4,0), sampler_ptr->clone());
    rectangle_ptr->set_material(green_ptr);
    add_object(rectangle_ptr);

    rectangle_ptr = new Rectangle(Vector3D(-7,0,0), Vector3D(2,0,0), Vector3D(0,4,0), sampler_ptr->clone());
    rectangle_ptr->set_material(green_ptr);
    add_object(rectangle_ptr);

    rectangle_ptr = new Rectangle(Vector3D(-0,0,0), Vector3D(2,0,0), Vector3D(0,4,0), sampler_ptr->clone());
    rectangle_ptr->set_material(green_ptr);
    add_object(rectangle_ptr);
}

void World::face_mesh_bvh_build(void){
    float light_radiance = 4.0; 

    // View Plane
    int lw = 50;
    vp = ViewPlane(lw, lw, 1.0, 1.0);

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
    pinhole_ptr->zoom = 2;
    this->camera_ptr = pinhole_ptr;


    vector<GeometricObject*> bvh_objects;

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.02));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    TriangleMesh* mesh = new TriangleMesh("face.obj");
    Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh->set_material(blue_ptr);
    bvh_objects.push_back(mesh);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::fast_mesh_build(void){
    float light_radiance = 4.0; 

    // View Plane
    int lw = 800;
    vp = ViewPlane(lw, lw, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(0, 0, -2));
    add_light(point_ptr);

    //Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(0, 0, -2);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 100;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = 8*4;
    this->camera_ptr = pinhole_ptr;


    vector<GeometricObject*> bvh_objects;

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.02));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    FastTriMesh* mesh = new FastTriMesh("face.obj");
    Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh->set_material(blue_ptr);
    bvh_objects.push_back(mesh);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::texture_build(void){
    // View Plane
    int lw = 400;
    vp = ViewPlane(lw, lw, 1.0, 1.0);
    // Background color
    background_color = GREEN / 10;
    // Tracer
    tracer_ptr = new RayCast(this);
    // Lights
    ambient_ptr = new Ambient(0.5, WHITE);
    Directional* dir_ptr = new Directional(2.5, WHITE, Vector3D(-15, 20, 25));
    add_light(dir_ptr);
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(-20, 10, 20);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 2, 0);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 1000;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = 1;
    this->camera_ptr = pinhole_ptr;


    // // image
    // Image* image_ptr = new Image;
    // image_ptr->read_ppm_file("face.ppm");
    // // Mapping
    // RectangularMap* map_ptr = new RectangularMap;
    // // Image texture 
    // ImageTexture* texture_ptr = new ImageTexture(image_ptr, map_ptr);
    // // Spatially Varying Material
    // SV_Matte* sv_matte_ptr = new SV_Matte(0.4, 0.95, texture_ptr);


    // Image texture 
    ConstantColor* texture_ptr = new ConstantColor(RED);
    // Spatially Varying Material
    SV_Matte* sv_matte_ptr = new SV_Matte(0.4, 0.95, texture_ptr);


    // Rectangle  
    Rectangle* rectangle_ptr = new Rectangle(Vector3D(-3,0,0), Vector3D(2,0,0), Vector3D(0,4,0));
    rectangle_ptr->set_material(sv_matte_ptr);
    add_object(rectangle_ptr);
    // Ground Plane 
    Matte* matte_ptr = new Matte(0.25, 0.5, 1.0);
    Plane* plane_ptr = new Plane(Vector3D(0, 0, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(matte_ptr);
    add_object(plane_ptr);

}

void World::animate(void){
    float light_radiance = 6.0; 

    // View Plane
    int lw = 300;
    vp = ViewPlane(lw, lw, 1.0, 1.0);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(0, 1, -2));
    add_light(point_ptr);
    point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(0, 1, 2));
    add_light(point_ptr);

    //Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(0, 0, 0);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 100;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = 1;
    this->camera_ptr = pinhole_ptr;

    vector<GeometricObject*> bvh_objects;

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.0));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    Deformed* mesh1 = new Deformed("face.obj");
    Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh1->set_material(blue_ptr);
    // bvh_objects.push_back(mesh1);

    // Instance* face = new Instance(mesh1);
    // Matte* green_ptr = new Matte(0.25, 0.75, GREEN);
    // face->set_material(green_ptr);
    // face->translate(Vector3D(0, 0, -0.3));
    // face->scale(1.5);
    // face->set_bounding_box();
    // bvh_objects.push_back(face);

    // face = new Instance(mesh1);
    // Matte* r_ptr = new Matte(0.25, 0.75, RED);
    // face->set_material(r_ptr);
    // face->translate(Vector3D(0, 0, 0.3));
    // face->scale(1.5);
    // face->set_bounding_box();
    // bvh_objects.push_back(face);

    
    // Create Materials
    vector<Material*> materials;
    Material* material_ptr = new Matte(0.25, 0.75, RED);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, YELLOW);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, GREEN);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, BLUE);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, CYAN);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, MAGENTA);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, BLACK);
    materials.push_back(material_ptr);
    material_ptr = new Matte(0.25, 0.75, WHITE);
    materials.push_back(material_ptr);

    // Load mesh
    // FastTriMesh* mesh = new FastTriMesh("face.obj");
    // mesh->set_material(material_ptr);

    Sphere* mesh = new Sphere(Vector3D(0,0,0), 0.1);

    // Create instances
    int ni = 10;
    vector<Instance*> instances(ni);
    // for (int i = 0; i < ni; i++){
    //     Instance* instance = new Instance(mesh);
    //     instance->set_material(materials[rand() % materials.size()]);
    //     instance->translate(Vector3D(i*.1,0,0.4));
    //     //instance->scale(1);
    //     //float theta = ((float)i / ni) * 2 * PI;
    //     //instance->rotate_y(theta);
    //     instance->set_bounding_box();
    //     bvh_objects.push_back(instance);
    // }

    for (int x = 0; x < ni; x++){
        for (int y = 0; y < ni; y++){
            for (int z = 0; z < ni; z++){
                Instance* instance = new Instance(mesh);
                instance->set_material(materials[rand() % materials.size()]);
                float z_coor = 0.4;
                if (z > (float)ni/2){
                    z_coor = -0.4;
                }
                instance->translate(Vector3D(x*.1,y*.1,z_coor));
                //instance->scale(1);
                //float theta = ((float)i / ni) * 2 * PI;
                //instance->rotate_y(theta);
                instance->set_bounding_box();
                bvh_objects.push_back(instance);
            }
        }
    }

    // Generate BVH build and add to world
    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);

    // Animation settings
    int num_frames = 200;
    string folder = "anim/";
    string ppm = ".ppm";
    string frame_name = "0.ppm";

    // Generate camera traversal
    vector<Vector3D> cam_pos(num_frames);
    cam_pos[0] = eye1;
    vector<Vector3D> look_pos(num_frames);
    look_pos[0] = lookat1;
    for (int i = 1; i < num_frames; i++){
        float theta = ((float)i / num_frames) * 2 * PI;
        Matrix rot = Matrix();
        rot.set_to_rot(theta, "y");
        // cam_pos[i] = rot * cam_pos[0];
        look_pos[i] = rot * look_pos[0];
    }

    // Render images
    for (int i = 0; i < num_frames; i++){
        // Change camera position
        //pinhole_ptr->eye = cam_pos[i];
        pinhole_ptr->lookat = look_pos[i];
        pinhole_ptr->compute_uvw();

        // Render and save scene
        this->render_scene();
        rename("image.ppm", frame_name.c_str());
        frame_name = folder + to_string(i) + ppm;
        cout << frame_name << endl;
    }

}

void World::checker_build(void){
    
    float light_radiance = 4.0;

    // View Plane
    vp = ViewPlane(1000, 1000, 1.0, 1.0, 8);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new Whitted(this);

    // Lights
    // Directional* directional_ptr = new Directional(light_radiance, WHITE, Vector3D(1, 1, 2));
    // add_light(directional_ptr);
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(12, 15, 30));
    add_light(point_ptr);
    // point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(7, 2, 6));
    // add_light(point_ptr);

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
    //Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);

    // BVH Objects
    vector<GeometricObject*> bvh_objects;

    // geometric Objects

    Sphere* sphere_ptr = new Sphere(Vector3D(0.0, 2.4, 0), 1.5);
    sphere_ptr->set_material(red_ptr);
    bvh_objects.push_back(sphere_ptr);

    Triangle* tri_ptr = new Triangle(Vector3D(1.5, -0.5, 1.8), Vector3D(7.5, -0.5, -9.00), Vector3D(2.35, 5.8, 1.4));
    tri_ptr->set_material(green_ptr);
    bvh_objects.push_back(tri_ptr);

    Plane* plane_ptr = new Plane(Vector3D(0, -0.5, 0), Vector3D(0, 1, 0));
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    // CHECKER SPHERE
    sphere_ptr = new Sphere(Vector3D(-4.0, 5, 6), 2);
    Checker* checker_ptr = new Checker(WHITE, BLACK, .2);
    SV_Matte* sv_matte = new SV_Matte(.5, .5, checker_ptr);
    sphere_ptr->set_material(sv_matte);
    bvh_objects.push_back(sphere_ptr);

    // // NOISE SPHERE
    // sphere_ptr = new Sphere(Vector3D(0.0, 8, 6), 3);
    // Noise* noise_ptr = new Noise(WHITE, BLACK, 30, sphere_ptr->get_bounding_box());
    // sv_matte = new SV_Matte(.5, .5, noise_ptr);
    // sphere_ptr->set_material(sv_matte);
    // bvh_objects.push_back(sphere_ptr);

    // CSG
    Sphere* s1 = new Sphere(Vector3D(8, 10, 6), 1);
    Phong* mat = new Phong(0.25, 0.75, 0.5, 3.0, RED);
    s1->set_material(mat);
    Sphere* s2 = new Sphere(Vector3D(7, 10, 6), 1);
    mat = new Phong(0.25, 0.75, 0.5, 3.0, GREEN);
    s2->set_material(mat);
    CSG* csg = new CSG(s1, s2, 0);
    csg->set_material(mat->clone());
    bvh_objects.push_back(csg);

    s1 = new Sphere(Vector3D(8, 7, 6), 1);
    mat = new Phong(0.25, 0.75, 0.5, 3.0, RED);
    s1->set_material(mat);
    s2 = new Sphere(Vector3D(7, 7, 6), 1);
    mat = new Phong(0.25, 0.75, 0.5, 3.0, GREEN);
    s2->set_material(mat);
    csg = new CSG(s1, s2, 1);
    csg->set_material(mat->clone());
    bvh_objects.push_back(csg);

    s1 = new Sphere(Vector3D(8, 4, 6), 1);
    mat = new Phong(0.25, 0.75, 0.5, 3.0, RED);
    s1->set_material(mat);
    s2 = new Sphere(Vector3D(7, 4, 6), 1);
    mat = new Phong(0.25, 0.75, 0.5, 3.0, GREEN);
    s2->set_material(mat);
    csg = new CSG(s2, s1, 2);
    csg->set_material(mat->clone());
    bvh_objects.push_back(csg);

    // s2 = new Sphere(Vector3D(7, 4, 6), 1);
    // mat = new Phong(0.25, 0.75, 0.5, 3.0, GREEN);
    // s2->set_material(mat);
    // bvh_objects.push_back(s2);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::glass_face_build(void){

    float light_radiance = 4.0; 

    // View Plane
    int lw = 800;
    vp = ViewPlane(lw, lw, 1.0, 1.0);

    // Sampler
    int n = 3;
    int num_samples = n*n;
    int num_sets = 83;
    Sampler* sampler_ptr = new Jittered(num_samples, num_sets);
    vp.set_sampler(sampler_ptr);

    // Ambient Light
    ambient_ptr = new Ambient(light_radiance * 0.5, WHITE);

    // Background color
    background_color = GREEN / 10;

    // Tracer
    tracer_ptr = new RayCast(this);

    // Lights
    Point* point_ptr = new Point(light_radiance * 1.0, WHITE, Vector3D(0, 0, -2));
    add_light(point_ptr);

    //Camera
    // Camera: Pinhole
    Pinhole* pinhole_ptr = new Pinhole();
    pinhole_ptr->up = Vector3D(0, -1, 0);
    Vector3D eye1(0, 0, -2);
    pinhole_ptr->eye = eye1;
    Vector3D lookat1(0, 0, -1);
    pinhole_ptr->lookat = lookat1;
    float distance1 = 100;
    pinhole_ptr->distance = distance1;
    pinhole_ptr->compute_uvw();
    pinhole_ptr->zoom = (lw/100)*4;
    this->camera_ptr = pinhole_ptr;


    vector<GeometricObject*> bvh_objects;

    Plane* plane_ptr = new Plane(Vector3D(0, -0.6, 0), Vector3D(0.0, 0.1, 0.02));
    Matte* yellow_ptr = new Matte(0.25, 0.75, YELLOW);
    plane_ptr->set_material(yellow_ptr);
    bvh_objects.push_back(plane_ptr);

    // GLASS/TRANSPARENT
    Transparent* trans_ptr = new Transparent(0.0, 0.0, 0.2, 2000, BLACK, 0.1, 0.9, .75);

    // FastTriMesh* mesh = new FastTriMesh("face.obj");
    SmoothMesh* mesh = new SmoothMesh("face.obj");
    //Phong* blue_ptr = new Phong(0.25, 0.75, 0.5, 3.0, BLUE);
    mesh->set_material(trans_ptr);
    bvh_objects.push_back(mesh);

    // Background Spheres
    Matte* blue_ptr = new Matte(0.25, 0.75, BLUE);
    Phong* red_ptr = new Phong(0.25, 0.75, 0.5, 3.0, RED);
    Matte* green_ptr = new Matte(0.25, 0.75, GREEN);

    Sphere* sphere_ptr = new Sphere(Vector3D(-0.3, -0.2, 2.5), .2);
    sphere_ptr->set_material(red_ptr);
    bvh_objects.push_back(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0.3, -0.2, 2.5), .2);
    sphere_ptr->set_material(blue_ptr);
    bvh_objects.push_back(sphere_ptr);

    sphere_ptr = new Sphere(Vector3D(0.0, 0.4, 2.5), .2);
    sphere_ptr->set_material(green_ptr);
    bvh_objects.push_back(sphere_ptr);

    BVH* bvh = new BVH(bvh_objects, bvh_objects.size(), 0);
    add_object(bvh);
}

void World::build(void){
    //cow_mesh_bvh_build();
    //shadow_bvh_build();
    //spheres_bvh_build();
    //cow_mesh_bvh_2_build();
    //cow_mesh_no_bvh_2_build();
    //spheres_bvh_build();
    //mirror_build();
    //transparent_build();
    //area_light_build();
    //face_mesh_bvh_build();
    //fast_mesh_build();
    //texture_build();
    //checker_build();
    glass_face_build();
}