#ifndef __CAMERA__
#define __CAMERA__

#include "Vector3D.h"

class World;

class Camera{
    public:
        Vector3D eye;
        Vector3D lookat;
        Vector3D up;
        Vector3D roll;
        float exposure_time;
        Vector3D u;
        Vector3D v;
        Vector3D w;

        Camera(void);
        Camera(Vector3D& eye, Vector3D& lookat, Vector3D& up, Vector3D& roll, float exposure_time);
        Camera(const Camera& camera);
        virtual ~Camera();
        Camera& operator=(const Camera& rhs);
        virtual Camera* clone(void) const = 0;
        
        virtual void render_scene(const World& w) = 0;
        void compute_uvw(void);
};

#endif