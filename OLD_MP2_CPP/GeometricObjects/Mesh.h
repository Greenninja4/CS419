#ifndef __MESH__
#define __MESH__

#include <vector>
#include "Vector3D.h"

using namespace std;

class Mesh{
    public:
        vector<Vector3D> vertices;
        vector<Vector3D> normals;
        vector<float> u;
        vector<float> v;
        vector<vector<int>> vertex_faces;
        int num_vertices;
        int num_triangles;

        Mesh(void);
        Mesh(const Mesh& mesh);
        ~Mesh(void);
        Mesh& operator=(const Mesh& rhs);
};

#endif