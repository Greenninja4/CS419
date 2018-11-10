#ifndef __PINHOLE__
#define __PINHOLE__

#include "Vector3D.h"
#include "Camera.h"
#include "World.h"

class Pinhole : public Camera {
    public:
        // Distance, zoom
        float distance;
        float zoom;

        Pinhole(void);
        Pinhole(Vector3D& eye, Vector3D& lookat, Vector3D& up, double& roll, float exposure_time, float distance, float zoom);
        Pinhole(const Pinhole& pinhole);
        virtual ~Pinhole();
        Pinhole& operator= (const Pinhole& rhs);
        virtual Camera* clone(void) const;
        
        virtual void render_scene(const World& w);
        Vector3D get_direction(const Vector3D& p) const;
};

#endif