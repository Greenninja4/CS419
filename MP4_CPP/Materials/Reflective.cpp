#include "Reflective.h"



Reflective::Reflective(void):
    Phong(),
    reflective_brdf(new PerfectSpecular) {}
Reflective::Reflective(const double& k_a, const double& k_d, const double& k_s, const double& exp, const Vector3D& color, const double& k_r): 
    Phong(k_a, k_d, k_s, exp, color),
    reflective_brdf(new PerfectSpecular(k_r, color)) {}
Reflective::Reflective(const Reflective& reflective):
    Phong(reflective) {
        if(reflective.reflective_brdf){
            reflective_brdf = reflective.reflective_brdf->clone();
        } else {
            reflective_brdf = NULL;
        }
    }
Reflective Reflective::operator= (const Reflective& rhs){
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
    return *this;
}
Reflective::~Reflective(void){
    if (reflective_brdf){
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
}
Material* Reflective::clone(void) const{
    return (new Reflective(*this));
}

Vector3D Reflective::shade(ShadeRec& sr){
    Vector3D L(Phong::shade(sr));
    
    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    Vector3D fr = reflective_brdf->sample_f(sr, wo, wi);
    Ray reflected_ray(sr.hit_point, wi);

    L = L + fr % sr.world.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);

    return L;
}
