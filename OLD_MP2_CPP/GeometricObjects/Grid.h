#ifndef __GRID__
#define __GRID__

#include "Compound.h"
#include "ShadeRec.h"
#include "Mesh.h"
#include "BBox.h"

class Grid: public Compound {
public:

    Grid(void);

    Grid(Mesh* _mesh_ptr);

    virtual Grid*
    clone(void) const;

    Grid(const Grid& rg);

    Grid&
    operator= (const Grid& rhs);

    virtual
    ~Grid(void);

    virtual BBox
    get_bounding_box(void);

    void
    read_flat_triangles(char* file_name);

    void
    read_smooth_triangles(char* file_name);

    void
    tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps);

    void
    tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps);

    virtual bool
    hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

    void
    setup_cells(void);

    void
    reverse_mesh_normals(void);

    void
    store_material(Material* material, const int index);

private:

    std::vector<GeometricObject*>	cells;
    int							nx, ny, nz;
    BBox						bbox;
    Mesh*						mesh_ptr;
    bool						reverse_normal;

    Vector3D
    find_min_bounds(void);

    Vector3D
    find_max_bounds(void);

    void
    read_ply_file(char* file_name, const int triangle_type);

    void
    compute_mesh_normals(void);
};

inline void
Grid::reverse_mesh_normals(void) {
    reverse_normal = true;
}

inline void
Grid::store_material(Material* material_ptr, const int index) {
    objects[index]->set_material(material_ptr);
}

#endif