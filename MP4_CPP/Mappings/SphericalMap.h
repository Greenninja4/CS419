#ifndef __SPHERICAL_MAP__
#define __SPHERICAL_MAP__

#include "Mapping.h"
#include "Constants.h"

class SphericalMap : public Mapping{
    public:
        // Big 6
        SphericalMap(void);
        SphericalMap(const SphericalMap& sm);
        SphericalMap& operator= (const SphericalMap& rhs);
        virtual Mapping* clone(void) const;
        ~SphericalMap(void);

        // Functions
        virtual void get_texel_coordinates(const Vector3D& hit_point, const int xres, const int yres, int& row, int& col) const;
};

#endif