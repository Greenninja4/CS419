#include "Deformed.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Big 6
Deformed::Deformed(void): 
    GeometricObject(), 
    num_vertices(0), 
    num_faces(0) {}
Deformed::Deformed(const string& file_name): 
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
Deformed::Deformed(const Deformed& object): 
    GeometricObject(object), 
    vertices(object.vertices), 
    faces(object.faces), 
    triangles(object.triangles), 
    num_vertices(0), 
    num_faces(0),
    bbox(object.bbox),
    bvh(object.bvh) {}
Deformed& Deformed::operator=(const Deformed& rhs){
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
Deformed::~Deformed(void){}
Deformed* Deformed::clone(void) const{
    return (new Deformed(*this));
}

// Getters & Setters
BBox Deformed::get_bounding_box(void){
    return bbox;
}
void Deformed::set_bounding_box(void){
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
bool Deformed::hit(const Ray& ray, double& tmin, ShadeRec& sr) const{
    return bvh.hit(ray, tmin, sr);
}
bool Deformed::shadow_hit(const Ray& ray, double& tmin) const{
    return bvh.shadow_hit(ray, tmin);
}

void Deformed::balloon(float rate){
    // Reserve enough bins for vertices
    int n = 10;
    vector<vector<int>> bins(n*n*n);
    // Get boundaries and size for mesh
    float x0,y0,z0,x1,y1,z1,xdiff,ydiff,zdiff,xcent,ycent,zcent;
    x0 = bbox.p0.x;
    y0 = bbox.p0.y;
    z0 = bbox.p0.z;
    x1 = bbox.p1.x;
    y1 = bbox.p1.y;
    z1 = bbox.p1.z;
    xdiff = x1-x0;
    ydiff = y1-y0;
    zdiff = z1-z0;
    xcent = x0 + xdiff/2;
    ycent = y0 + ydiff/2;
    zcent = z0 + zdiff/2;
    Vector3D center = Vector3D(xcent, ycent, zcent);
    // Bin each vertex based on locaion
    for (int i = 0; i < num_vertices; i++){
        int xi,yi,zi,index;
        xi = (int)(n*(vertices[i].x - x0)/xdiff);
        yi = (int)(n*(vertices[i].y - y0)/ydiff);
        zi = (int)(n*(vertices[i].z - z0)/zdiff);
        index = xi + n*yi + n*n*zi;
        bins[index].push_back(i);
    }
    // For each bin
    for (int i = 0; i < n*n*n; i++){
        // Find bin indexes
        int xi,yi,zi;
        zi = (int)(i/(n*n));
        yi = (int)((i-zi*n*n)/n);
        xi = (int)(i-zi*n*n-yi*n);
        // For each vertex in bin
        for (int j = 0; j < bins[i].size() ; j++){
            // Spread out
            if (yi <= 2){
                Vector3D dir = vertices[bins[i][j]] - center;
                Vector3D delta = rate * (1/dir.length()) * dir.hat();
                Vector3D newpos = vertices[bins[i][j]] + delta;
                vertices[bins[i][j]] = newpos;
            }
            // Fall down
            else {
                Vector3D dir = Vector3D(0,-1,0);
                Vector3D delta = rate * dir.hat();
                Vector3D newpos = vertices[bins[i][j]] + delta;
                vertices[bins[i][j]] = newpos;
            }
        }
    }
    recalculate_triangles();
}

void Deformed::recalculate_triangles(void){
    triangles.clear();
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