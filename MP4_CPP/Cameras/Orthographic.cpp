#include "Orthographic.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

Orthographic::Orthographic(void):
    Camera(), 
    distance(500), 
    zoom(1.0) {}
Orthographic::Orthographic(Vector3D& eye, Vector3D& lookat, Vector3D& up, double& roll, float exposure_time, float distance, float zoom): 
    Camera(), 
    distance(distance), 
    zoom(zoom) {
    this->eye = eye;
    this->lookat = lookat;
    this->up = up;
    this->roll = roll;
    this->exposure_time = exposure_time;
    compute_uvw();
}
Orthographic::Orthographic(const Orthographic& Orthographic): 
    Camera(Orthographic), 
    distance(Orthographic.distance), 
    zoom(Orthographic.zoom) {}
Orthographic::~Orthographic() {}
Orthographic& Orthographic::operator= (const Orthographic& rhs) {
    if (this == &rhs){
        return *this;
    }
    eye = rhs.eye;
    lookat = rhs.lookat;
    up = rhs.up;
    roll = rhs.roll;
    exposure_time = rhs.exposure_time;
    u = rhs.u;
    v = rhs.v;
    w = rhs.w;
    distance = rhs.distance;
    zoom = rhs.zoom;
    return *this;
}
Camera* Orthographic::clone(void) const{
    return (new Orthographic(*this));
}

void Orthographic::render_scene(const World& w){
    Vector3D color;
    ViewPlane vp(w.vp);
    Ray ray;
    Vector3D pp;

    vp.pixelSize /= zoom;
    ray.d = Vector3D(0, 0, -1);
    ray.d.normalize();
    double x, y, zw = 100.0;
    
    ofstream file;
    file.open("image.ppm");
    file << "P3 " << vp.hres << " " << vp.vres << " " << 255.0 << "\n";

    for (int r = 0; r < vp.vres; r++){
        for (int c = 0; c < vp.hres; c++){
            x = vp.pixelSize * (c - 0.5 * (vp.hres - 1.0));
            y = vp.pixelSize * (r - 0.5 * (vp.vres - 1.0));
            //zw = eye.z;
            ray.o = Vector3D(x, y, zw);
            // cout << "Origin: " << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << ", " << endl;
            // cout << "Direction: " << ray.d.x << ", " << ray.d.y << ", " << ray.d.z << ", " << endl;
            color = w.tracer_ptr->trace_ray(ray);
            color = color * exposure_time;
            color = w.max_to_one(color);
            color = color * 255;
            file << (int) color.x << " " << (int) color.y << " " << (int) color.z << "\t";
        }
        file << "\n";
    }
    file.close();
}
