#include "Phong.h"
#include <iostream>

Phong::Phong(void): 
    Material(), 
    ambient_brdf(new Lambertian), 
    diffuse_brdf(new Lambertian){}
Phong::Phong(float k_a, float k_d, Vector3D color): 
    Material(), 
    ambient_brdf(new Lambertian(k_a, color)), 
    diffuse_brdf(new Lambertian(k_d, color)){}
Phong::Phong(const Phong& phong): 
    Material(phong){
    if(phong.ambient_brdf){
        ambient_brdf = phong.ambient_brdf->clone();
    } else{
        ambient_brdf = NULL;
    }

    if(phong.diffuse_brdf){
        diffuse_brdf = phong.diffuse_brdf->clone();
    } else{
        diffuse_brdf = NULL;
    }
}
Phong& Phong::operator=(const Phong& rhs){
    if(this == &rhs){
        return *this;
    }

    Material::operator=(rhs);

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
Phong::~Phong(void){
    if (ambient_brdf){
        delete ambient_brdf;
        ambient_brdf = NULL;
    }
    if (diffuse_brdf){
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
}
Material* Phong::clone(void) const{
    return (new Phong(*this));
}

Vector3D Phong::shade(ShadeRec& sr){
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