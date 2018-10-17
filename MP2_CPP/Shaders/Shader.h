#ifndef __SHADER__
#define __SHADER__

#include "Vector3D.h"

class Shader{
    public:
        Shader(void);
        ~Shader(void);
        Vector3D shade(Vector3D i);
};

#endif