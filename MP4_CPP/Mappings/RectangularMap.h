#ifndef __RECTANGULAR_MAP__
#define __RECTANGULAR_MAP__

#include "Mapping.h"
#include "Constants.h"

class RectangularMap : public Mapping{
    public:
        // Big 6
        RectangularMap(void);
        RectangularMap(const RectangularMap& rm);
        RectangularMap& operator= (const RectangularMap& rhs);
        virtual Mapping* clone(void) const;
        ~RectangularMap(void);

        // Functions
        virtual void get_texel_coordinates(const Vector3D& hit_point, const int xres, const int yres, int& row, int& col) const;
};

#endif