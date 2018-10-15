#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

#include "Vector3D.h"
#include "Ray.h"
#include "ShadeRec.h"

class GeometricObject{
    public:
        // Color
        Vector3D color;

        GeometricObject(void);
        GeometricObject(const GeometricObject& object);
        GeometricObject& operator=(const GeometricObject& rhs);
        virtual ~GeometricObject(void);

        // Ray hits an object?
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
};

#endif