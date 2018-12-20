#ifndef __MAPPING__
#define __MAPPING__

#include "Vector3D.h"

class Mapping{
    public:
        // Big 6
        Mapping(void);
        Mapping(const Mapping& m);
        Mapping& operator= (const Mapping& rhs);
        virtual Mapping* clone(void) const = 0;
        virtual ~Mapping(void);

        // Functions
        virtual void get_texel_coordinates(const Vector3D& hit_point, const int xres, const int yres, int& row, int& col) const = 0;
};

#endif