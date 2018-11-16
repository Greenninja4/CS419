#include "Matte.h"
#include <iostream>

// Big 6
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

// Functions
Vector3D Matte::shade(ShadeRec& sr){
    Vector3D wo = -sr.ray.d;
    Vector3D L = ambient_brdf->rho(sr, wo) % sr.world.ambient_ptr->L(sr);
    int num_lights = sr.world.lights.size();

    for (int j = 0; j < num_lights; j++){
        Vector3D wi = sr.world.lights[j]->get_direction(sr);
        double ndotwi = sr.normal * wi;

        if (ndotwi > 0.0){

            bool in_shadow = false;
            if (sr.world.lights[j]->casts_shadow()){
                Ray shadow_ray(sr.hit_point, wi);
                in_shadow = sr.world.lights[j]->in_shadow(shadow_ray, sr);
            }

            if (!in_shadow){
                L = L + diffuse_brdf->f(sr, wo, wi) % sr.world.lights[j]->L(sr) * ndotwi;
            }
        }
    }
    return L;
}

Vector3D Matte::area_light_shade(ShadeRec& sr){
    Vector3D wo = -sr.ray.d;
    Vector3D L = ambient_brdf->rho(sr, wo) % sr.world.ambient_ptr->L(sr);
    int num_lights = sr.world.lights.size();

    for (int j = 0; j < num_lights; j++){
        Vector3D wi = sr.world.lights[j]->get_direction(sr);
        double ndotwi = sr.normal * wi;
        
        if (ndotwi > 0.0){

            bool in_shadow = false;
            if (sr.world.lights[j]->casts_shadow()){
                Ray shadow_ray(sr.hit_point, wi);
                in_shadow = sr.world.lights[j]->in_shadow(shadow_ray, sr);
            }

            if (!in_shadow){
                // float G = sr.world.lights[j]->G(sr);
                // cout << "G: " << G << ", ";
                // float pdf = sr.world.lights[j]->pdf(sr);
                // cout << "PDF: " << pdf << ", " << endl;
                // cout << L.x << ", " << L.y << ", " << L.z << "...";
                L = L + (diffuse_brdf->f(sr, wo, wi) % sr.world.lights[j]->L(sr) * ndotwi)
                * sr.world.lights[j]->G(sr) /
                sr.world.lights[j]->pdf(sr);

                // cout << L.x << ", " << L.y << ", " << L.z << "..." << endl;
                // L = L + diffuse_brdf->f(sr, wo, wi) % sr.world.lights[j]->L(sr)
                // * sr.world.lights[j]->G(sr) * ndotwi /
                // sr.world.lights[j]->pdf(sr);
                // cout << L.x << ", " << L.y << ", " << L.z << endl;
            }
        }
    }
    return L;
}
