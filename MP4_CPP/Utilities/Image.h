#ifndef __IMAGE__
#define __IMAGE__

#include <vector>
#include "Vector3D.h"
using namespace std;

class Image {
    public:
        // Variables
        int hres;
        int vres;
        vector<Vector3D> pixels;

        // Big 6
        Image(void);
        Image(const Image& image);
        Image& operator= (const Image& rhs);
        ~Image(void);

        // Functions
        void read_ppm_file(const char* file_name);
        Vector3D get_color(const int row, const int column) const;
};

#endif