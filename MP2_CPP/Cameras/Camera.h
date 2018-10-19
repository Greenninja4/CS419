#ifndef __CAMERA__
#define __CAMERA__

#include "Vector3D.h"

class World; // World contains a Camera*, so must use forward declaration

class Camera{
    public:
        // Variables
        Vector3D eye;           // Eye point
        Vector3D lookat;        // Lookat point
        Vector3D up;            // Up vector (usually (0, 1, 0) in world coordinates)
        Vector3D roll;          // Roll 
        float exposure_time;    // Exposure time
        Vector3D u, v, w;       // Orthonormal basis for viewing coordinates

        Camera(void);
        Camera(const Vector3D& eye, const Vector3D& lookat, const Vector3D& up, const Vector3D& roll, float exposure_time);
        Camera(const Camera& camera);
        virtual ~Camera();
        Camera& operator= (const Camera& rhs);
        virtual Camera* clone(void) const = 0;
        
        virtual void render_scene(const World& w) = 0;
        void compute_uvw(void);
};

#endif