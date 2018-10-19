#include "Mesh.h"

Mesh::Mesh(void): 
    num_vertices(0), 
    num_triangles(0) {}
Mesh::Mesh(const Mesh& mesh):
    vertices(mesh.vertices),
    normals(mesh.normals), 
    u(mesh.u), 
    v(mesh.v), 
    num_vertices(mesh.num_vertices), 
    num_triangles(mesh.num_triangles) {}
Mesh::~Mesh(void){}
Mesh& Mesh::operator=(const Mesh& rhs){
    if (this == &rhs){
        return *this;
    }
    vertices = rhs.vertices;
    normals = rhs.normals;
    u = rhs.u;
    v = rhs.v;
    num_vertices = rhs.num_vertices;
    num_triangles = rhs.num_triangles;
    return *this;
}