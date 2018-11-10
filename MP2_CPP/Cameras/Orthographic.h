#ifndef __ORTHOGRAPHIC__
#define __ORTHOGRAPHIC__

#include "Vector3D.h"
#include "Camera.h"
#include "World.h"

class Orthographic : public Camera {
    public:
        // Distance, zoom
        float distance;
        float zoom;

        Orthographic(void);
        Orthographic(Vector3D& eye, Vector3D& lookat, Vector3D& up, double& roll, float exposure_time, float distance, float zoom);
        Orthographic(const Orthographic& orthographic);
        virtual ~Orthographic();
        Orthographic& operator= (const Orthographic& rhs);
        virtual Camera* clone(void) const;
        
        virtual void render_scene(const World& w);
};

#endif