#ifndef __NOISE__
#define __NOISE__

#include "Texture.h"
#include <vector>
#include "BBox.h"

using namespace std;

class Noise : public Texture {
    public: 
        // Variables
        Vector3D color1;
        Vector3D color2;
        float size;
        vector<float> lattice;
        BBox bbox;
        // Lattice goes from 0->size in all directions, need to scale by bbox
        

        // Big 6
        Noise(void);
        Noise(const Vector3D& color1, const Vector3D& color2, const float& size, const BBox& bbox);
        Noise(const Noise& noise);
        virtual Texture* clone(void) const;
        virtual ~Noise(void);
        Noise& operator= (const Noise& rhs);

        // Functions
        virtual Vector3D get_color(const ShadeRec& sr) const;
        void generate_lattice(void);
        float lattice_point(int x, int y, int z) const;
        float lattice_lerp(const Vector3D& p) const;
};

#endif