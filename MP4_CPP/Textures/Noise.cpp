#include "Noise.h"
#include "Constants.h"
#include "Maths.h"

#include <iostream>
using namespace std;

// Big 6
Noise::Noise(void): 
    color1(BLACK),
    color2(WHITE),
    size(1.0),
    bbox(9999999,9999999) {
    generate_lattice();
}
Noise::Noise(const Vector3D& color1, const Vector3D& color2, const float& size, const BBox& bbox): 
    color1(color1),
    color2(color2),
    size(size),
    bbox(bbox) {
    generate_lattice();
}
Noise::Noise(const Noise& noise):
    color1(noise.color1),
    color2(noise.color2),
    size(noise.size),
    lattice(noise.lattice),
    bbox(noise.bbox) {}
Texture* Noise::clone(void) const{
    return (new Noise(*this));
}
Noise::~Noise(void){}
Noise& Noise::operator= (const Noise& rhs){
    if (this == &rhs){
        return *this;
    }
    Texture::operator= (rhs);
    color1 = rhs.color1;
    color2 = rhs.color2;
    size = rhs.size;
    lattice = rhs.lattice;
    bbox = rhs.bbox;
    return *this;
}

// Functions
Vector3D Noise::get_color(const ShadeRec& sr) const{

    // Put local hit point into lattice coordinates
    Vector3D hp = sr.hit_point;
    Vector3D axes = bbox.p1 - bbox.p0; // Lenghts of bbox edges in world coors

    float x = size * (hp.x - bbox.p0.x) / axes.x;
    float y = size * (hp.y - bbox.p0.y) / axes.y;
    float z = size * (hp.z - bbox.p0.z) / axes.z;
    Vector3D lattice_point = Vector3D(x, y, z); // Lattice point coordinates

    // Get color
    float lattice_val = lattice_lerp(lattice_point);
    
    // cout << lattice_val << endl;
    
    Vector3D out_color = (lattice_val)*color1 + (1 - lattice_val)*color2;

    // cout << out_color.x << ", " << out_color.y << ", " << out_color.z << endl;

    return out_color;
}


void Noise::generate_lattice(void){
    lattice.clear();
    lattice.resize(size*size*size, 0);
    for (int i = 0; i < size*size*size; i++){
        lattice[i] = rand_float();
    }
}

float Noise::lattice_point(int x, int y, int z) const{
    return lattice[x + y*size + z*size*size];
}

float Noise::lattice_lerp(const Vector3D& p) const{
    // Lattice coordinates (min and max xyz values from 0->size of lattice)
    int x0,x1,y0,y1,z0,z1;
    x0 = (int)(p.x);
    y0 = (int)(p.y);
    z0 = (int)(p.z);
    x1 = x0 + 1;
    y1 = y0 + 1;
    z1 = z0 + 1;

    // Lattice points (0->7 corresponds to xyz = 000,001,010,011,100,101,110,111)
    vector<float> vec;
    vec.push_back(lattice_point(x0,y0,z0));
    vec.push_back(lattice_point(x0,y0,z1));
    vec.push_back(lattice_point(x0,y1,z0));
    vec.push_back(lattice_point(x0,y1,z1));
    vec.push_back(lattice_point(x1,y0,z0));
    vec.push_back(lattice_point(x1,y0,z1));
    vec.push_back(lattice_point(x1,y1,z0));
    vec.push_back(lattice_point(x1,y1,z1));

    // First round of interpolation (along z-direction)
    float a,b,c,d;
    a = (p.z - z0)*vec[1] + (z1 - p.z)*vec[0];
    b = (p.z - z0)*vec[3] + (z1 - p.z)*vec[2];
    c = (p.z - z0)*vec[5] + (z1 - p.z)*vec[4];
    d = (p.z - z0)*vec[7] + (z1 - p.z)*vec[6];

    // Second round of interpolation (along y-direction)
    float ab, cd;
    ab = (p.y - y0)*b + (y1 - p.y)*a;
    cd = (p.y - y0)*d + (y1 - p.y)*c;

    // Third round of interpolation (along x-direction)
    float abcd;
    abcd = (p.x - x0)*cd + (x1 - p.x)*ab;

    return abcd;
}