#ifndef __COMPOUND__
#define __COMPOUND__

#include "GeometricObject.h"
#include <vector>
#include "Material.h"

class Compound : public GeometricObject{
    public:
        // Variables
        std::vector<GeometricObject*> objects;
        BBox bbox;

        // Big 6
        Compound(void);
        Compound(const Compound& Compound);
        Compound& operator= (const Compound& rhs);
        ~Compound (void);
        virtual Compound* clone(void) const;

        // Functions
        virtual void add_object(GeometricObject* object_ptr);
        int get_num_object();
        void copy_objects(std::vector<GeometricObject*> objects);
        void delete_objects(void);
        void set_material(Material* material_ptr);
		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
        virtual bool shadow_hit(const Ray& ray, double& tmin) const;
        virtual BBox get_bounding_box(void);
        virtual void set_bounding_box(void);
};
#endif