#include "Transparent.h"

Transparent::Transparent(void):
    Phong(),
    reflective_brdf(new PerfectSpecular),
    specular_btdf(new PerfectTransmitter) {}
Transparent::Transparent(const double& k_a, const double& k_d, const double& k_s, const double& exp, const Vector3D& color, const double& k_r, const double& kt, const double& ior): 
    Phong(k_a, k_d, k_s, exp, color),
    reflective_brdf(new PerfectSpecular(k_r, color)),
    specular_btdf(new PerfectTransmitter(kt, ior)) {}
Transparent::Transparent(const Transparent& transparent):
    Phong(transparent) {
    if(transparent.reflective_brdf){
        reflective_brdf = transparent.reflective_brdf->clone();
    } else {
        reflective_brdf = NULL;
    }
    if(transparent.specular_btdf){
        specular_btdf = transparent.specular_btdf->clone();
    } else {
        specular_btdf = NULL;
    }
}
Transparent Transparent::operator= (const Transparent& rhs){
    if (this == &rhs){
        return *this;
    }
    Phong::operator=(rhs);
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
    if (rhs.reflective_brdf){
        reflective_brdf = rhs.reflective_brdf->clone();
    }
    if (specular_btdf) {
        delete specular_btdf;
        specular_btdf = NULL;
    }
    if (rhs.specular_btdf){
        specular_btdf = rhs.specular_btdf->clone();
    }
    return *this;
}
Transparent::~Transparent(void){
    if (reflective_brdf){
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
    if (specular_btdf){
        delete specular_btdf;
        specular_btdf = NULL;
    }
}
Material* Transparent::clone(void) const{
    return (new Transparent(*this));
}

Vector3D Transparent::shade(ShadeRec& sr){
    Vector3D L(Phong::shade(sr));
    
    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    Vector3D fr = reflective_brdf->sample_f(sr, wo, wi);
    Ray reflected_ray(sr.hit_point, wi);

    if(specular_btdf->tir(sr)){
        L = L + sr.world.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1);
    } else {
        Vector3D wt;
        Vector3D ft = specular_btdf->sample_f(sr, wo, wt);
        Ray transmitted_ray(sr.hit_point, wt);
        L = L + fr % sr.world.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * fabs(sr.normal * wi);
        L = L + ft % sr.world.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1) * fabs(sr.normal * wt);
    }
    return L;
}
