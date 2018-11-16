#include "Pinhole.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

Pinhole::Pinhole(void):
    Camera(), 
    distance(500), 
    zoom(1.0) {}
Pinhole::Pinhole(Vector3D& eye, Vector3D& lookat, Vector3D& up, double& roll, float exposure_time, float distance, float zoom): 
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
Pinhole::Pinhole(const Pinhole& pinhole): 
    Camera(pinhole), 
    distance(pinhole.distance), 
    zoom(pinhole.zoom) {}
Pinhole::~Pinhole() {}
Pinhole& Pinhole::operator= (const Pinhole& rhs) {
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
Camera* Pinhole::clone(void) const{
    return (new Pinhole(*this));
}

Vector3D Pinhole::get_direction(const Vector3D& p) const{
    Vector3D dir = p.x * u + p.y * v - distance * w;
    dir.normalize();
    return dir;
}

void Pinhole::render_scene(const World& w){
    Vector3D color;
    ViewPlane vp(w.vp);
    Ray ray;
    Vector3D pp;

    vp.pixelSize /= zoom;
    ray.o = eye;

    ofstream file;
    file.open("image.ppm");
    file << "P3 " << vp.hres << " " << vp.vres << " " << 255.0 << "\n";

    // For sample points
    Vector3D sp;
    if (vp.sampler_ptr){
        for (int r = 0; r < vp.vres; r++){
            for (int c = 0; c < vp.hres; c++){
                color = BLACK;
                for (int j = 0; j < vp.sampler_ptr->num_samples; j++){
                    sp = vp.sampler_ptr->sample_unit_square();
                    pp.x = vp.pixelSize * (c - 0.5 * vp.hres + sp.x);
                    pp.y = vp.pixelSize * (r - 0.5 * vp.vres + sp.y);
                    ray.d = get_direction(pp);
                    color = color + w.tracer_ptr->trace_ray(ray);
                }
                color = color / vp.sampler_ptr->num_samples;
                color = color * exposure_time;
                color = w.max_to_one(color);
                file << (int) (255 * color.x) << " " << (int) (255 * color.y) << " " << (int) (255 * color.z) << "\t";
            }
            file << "\n";
        }
    // No sampler
    } else {
        for (int r = 0; r < vp.vres; r++){
            for (int c = 0; c < vp.hres; c++){
                pp.x = vp.pixelSize * (c - 0.5 * (vp.hres - 1.0));
                pp.y = vp.pixelSize * (r - 0.5 * (vp.vres - 1.0));
                if(vp.sampler_ptr){
                    sp = vp.sampler_ptr->sample_unit_square();
                    pp.x = vp.pixelSize * (c - 0.5 * vp.hres + sp.x);
                    pp.y = vp.pixelSize * (r - 0.5 * vp.vres + sp.y);
                }
                ray.d = get_direction(pp);
                color = w.tracer_ptr->trace_ray(ray);
                color = color * exposure_time;
                color = w.max_to_one(color);
                file << (int) (255 * color.x) << " " << (int) (255 * color.y) << " " << (int) (255 * color.z) << "\t";
            }
            file << "\n";
        }
    }

    file.close();
}
