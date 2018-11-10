#include "Phong.h"

// Big 6
Phong::Phong(void): 
    Material(), 
    ambient_brdf(new Lambertian), 
    diffuse_brdf(new Lambertian),
    specular_brdf(new GlossySpecular) {}
Phong::Phong(const double& k_a, const double &k_d, const double& k_s, const double& exp, const Vector3D& color): 
    Material(), 
    ambient_brdf(new Lambertian(k_a, color)), 
    diffuse_brdf(new Lambertian(k_d, color)), 
    specular_brdf(new GlossySpecular(k_s, color, exp)) {}
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

    if(phong.specular_brdf){
        specular_brdf = phong.specular_brdf->clone();
    } else{
        specular_brdf = NULL;
    }
}
Phong& Phong::operator= (const Phong& rhs){
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

    if(specular_brdf){
        delete specular_brdf;
        specular_brdf = NULL;
    }
    if (rhs.specular_brdf){
        specular_brdf = rhs.specular_brdf->clone();
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
    if (specular_brdf){
        delete specular_brdf;
        specular_brdf = NULL;
    }
}
Material* Phong::clone(void) const{
    return (new Phong(*this));
}

// Functions
Vector3D Phong::shade(ShadeRec& sr){
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
                L = L + (diffuse_brdf->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi))
                     % (sr.world.lights[j]->L(sr)) * ndotwi;
            }
        }
    }
    return L;
}