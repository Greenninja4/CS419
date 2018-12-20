#include "FastTriMesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Big 6
FastTriMesh::FastTriMesh(void): 
    GeometricObject(), 
    num_vertices(0), 
    num_faces(0) {}
FastTriMesh::FastTriMesh(const string& file_name): 
    GeometricObject(), 
    num_vertices(0), 
    num_faces(0) {
    string line;
    ifstream obj;
    obj.open(file_name);
    if (obj.is_open()){
        while (getline (obj, line)){
            // Vertices
            if (line.substr(0,2) == "v "){
                num_vertices += 1;
                istringstream v(line.substr(2));
                double x, y, z;
                v>>x; v>>y; v>>z;
                Vector3D vert(x, y, z);
                vertices.push_back(vert);
            }
            // Faces
            if (line.substr(0,2) == "f "){
                num_faces += 1;
                istringstream f(line.substr(2));
                int a, b, c;
                f>>a; f>>b; f>>c;
                a--; b--; c--;
                vector<int> face;
                face.push_back(a); face.push_back(b); face.push_back(c);
                faces.push_back(face);
            }
        }
        obj.close();

        // Triangles
        for (int i = 0; i < faces.size(); i++){
            Vector3D v0(vertices[faces[i][0]].x, vertices[faces[i][0]].y, vertices[faces[i][0]].z);
            Vector3D v1(vertices[faces[i][1]].x, vertices[faces[i][1]].y, vertices[faces[i][1]].z);
            Vector3D v2(vertices[faces[i][2]].x, vertices[faces[i][2]].y, vertices[faces[i][2]].z);
            Triangle* tri = new Triangle(v0, v1, v2);
            triangles.push_back(tri);
        }

        // Bounding Box
        set_bounding_box();

        // Create BVH
        bvh = BVH(triangles, triangles.size(), 0);
    }
    else cout << "Can't open " << file_name << endl;
}
FastTriMesh::FastTriMesh(const FastTriMesh& object): 
    GeometricObject(object), 
    vertices(object.vertices), 
    faces(object.faces), 
    triangles(object.triangles), 
    num_vertices(0), 
    num_faces(0),
    bbox(object.bbox),
    bvh(object.bvh) {}
FastTriMesh& FastTriMesh::operator=(const FastTriMesh& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    vertices = rhs.vertices;
    faces = rhs.faces;
    triangles = rhs.triangles;
    num_vertices = rhs.num_vertices;
    num_faces = rhs.num_faces;
    bbox = rhs.bbox;
    bvh = rhs.bvh;
    return *this;
}
FastTriMesh::~FastTriMesh(void){}
FastTriMesh* FastTriMesh::clone(void) const{
    return (new FastTriMesh(*this));
}

// Getters & Setters
BBox FastTriMesh::get_bounding_box(void){
    return bbox;
}
void FastTriMesh::set_bounding_box(void){
    double x0, y0, z0, x1, y1, z1;
    double vx, vy, vz;
    x0 = vertices[0].x;
    x1 = x0;
    y0 = vertices[0].y;
    y1 = x0;
    z0 = vertices[0].z;
    z1 = x0;
    for (int i = 0; i < num_vertices; i++){
        vx = vertices[i].x;
        vy = vertices[i].y;
        vz = vertices[i].z;
        if (vertices[i].x < x0){
            x0 = vertices[i].x;
        }
        if (vertices[i].x > x1){
            x1 = vertices[i].x;
        }
        if (vertices[i].y < y0){
            y0 = vertices[i].y;
        }
        if (vertices[i].y > y1){
            y1 = vertices[i].y;
        }
        if (vertices[i].z < z0){
            z0 = vertices[i].z;
        }
        if (vertices[i].z > z1){
            z1 = vertices[i].z;
        }
    }
    bbox = BBox(Vector3D(x0, y0, z0), Vector3D(x1, y1, z1));
}

// Functions
bool FastTriMesh::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    return bvh.hit(ray, tmin, sr);
}
bool FastTriMesh::shadow_hit(const Ray& ray, double& tmin) const{
    return bvh.shadow_hit(ray, tmin);
}