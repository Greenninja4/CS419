#include "Matte.h"
#include <iostream>

Matte::Matte(void): 
    Material(), 
    ambient_brdf(new Lambertian), 
    diffuse_brdf(new Lambertian){}
Matte::Matte(float k_a, float k_d, Vector3D color): 
    Material(), 
    ambient_brdf(new Lambertian(k_a, color)), 
    diffuse_brdf(new Lambertian(k_d, color)){}
Matte::Matte(const Matte& matte): 
    Material(matte){
    if(matte.ambient_brdf){
        ambient_brdf = matte.ambient_brdf->clone();
    } else{
        ambient_brdf = NULL;
    }

    if(matte.diffuse_brdf){
        diffuse_brdf = matte.diffuse_brdf->clone();
    } else{
        diffuse_brdf = NULL;
    }
}
Matte& Matte::operator= (const Matte& rhs){
    if(this == &rhs){
        return *this;
    }

    Material::operator= (rhs);

    if(ambient_brdf){
        delete ambient_brdf;
        ambient_brdf = NULL;
    }
    if (rhs.ambient_brdf){
        ambient_brdf = rhs.ambient_brdf->clone();
    }

    if(diffuse_brdf){
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
    if (rhs.diffuse_brdf){
        diffuse_brdf = rhs.diffuse_brdf->clone();
    }

    return (*this);
}
Matte::~Matte(void){
    if (ambient_brdf){
        delete ambient_brdf;
        ambient_brdf = NULL;
    }
    if (diffuse_brdf){
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
}
Material* Matte::clone(void) const{
    return (new Matte(*this));
}

Vector3D Matte::shade(ShadeRec& sr){
    Vector3D wo = -sr.ray.d;
    Vector3D L = ambient_brdf->rho(sr, wo) % sr.world.ambient_ptr->L(sr);
    int num_lights = sr.world.lights.size();
    if (num_lights > 0)
        // std::cout << "Num lights: " << num_lights << std::endl;

    for (int j = 0; j < num_lights; j++){
        Vector3D wi = sr.world.lights[j]->get_direction(sr);
        double ndotwi = sr.normal * wi;

        if (ndotwi > 0.0){
            L = L + diffuse_brdf->f(sr, wo, wi) % sr.world.lights[j]->L(sr) * ndotwi;
        }
    }
    return L;
}