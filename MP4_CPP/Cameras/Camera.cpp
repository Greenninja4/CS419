#include "Camera.h"

Camera::Camera(void):
    eye(0, 0, 500), 
    lookat(0, 0, 0), 
    up(0, 1, 0), 
    roll(0), 
    exposure_time(1.0), 
    u(1, 0, 0), 
    v(0, 1, 0), 
    w(0, 0, 1) {}
Camera::Camera(const Vector3D& eye, const Vector3D& lookat, const Vector3D& up, const double& roll, float exposure_time): 
    eye(eye), 
    lookat(lookat), 
    up(up), 
    roll(roll), 
    exposure_time(exposure_time) {
    compute_uvw();
}
Camera::Camera(const Camera& camera): 
    eye(camera.eye), 
    lookat(camera.lookat), 
    up(camera.up), 
    roll(camera.roll), 
    exposure_time(camera.exposure_time), 
    u(camera.u), 
    v(camera.v), 
    w(camera.w) {}
Camera::~Camera() {}
Camera& Camera::operator= (const Camera& rhs) {
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
    return *this;
}

void Camera::compute_uvw(void){
    w = eye - lookat;
    w.normalize();
    u = up^w;
    u.normalize();
    v = w^u;

    // Singularity (divide by zero)
    // Looking Up (+y-axis)
    if ( (eye.x == lookat.x) && (eye.z == lookat.z) && (eye.y > lookat.y) ){
        u = Vector3D(0, 0, 1);
        v = Vector3D(1, 0, 0);
        w = Vector3D(0, 1, 0);
    }
    // Looking Down (-y-axis)
    if ( (eye.x == lookat.x) && (eye.z == lookat.z) && (eye.y < lookat.y) ) {
        u = Vector3D(1, 0, 0);
        v = Vector3D(0, 0, 1);
        w = Vector3D(0, -1, 0);
    }
}