#include "SV_Lambertian.h"

#include "Constants.h"

SV_Lambertian::SV_Lambertian(void): 
    BRDF(), 
    k_d(0.0), 
    texture_ptr(NULL){}
SV_Lambertian::SV_Lambertian(float k_d, Texture* texture_ptr):
    BRDF(), 
    k_d(k_d), 
    texture_ptr(texture_ptr){}
SV_Lambertian::SV_Lambertian(const SV_Lambertian& sv_lambertian): 
    BRDF(sv_lambertian), 
    k_d(sv_lambertian.k_d), 
    texture_ptr(sv_lambertian.texture_ptr){}
SV_Lambertian::~SV_Lambertian(void){
    if (texture_ptr){
        delete texture_ptr;
        texture_ptr = NULL;
    }
}
SV_Lambertian& SV_Lambertian::operator= (const SV_Lambertian& rhs){
    if (this == &rhs){
        return *this;
    }
    BRDF::operator= (rhs);
    k_d = rhs.k_d;

    if(texture_ptr){
        delete texture_ptr;
        texture_ptr = NULL;
    }
    if (rhs.texture_ptr){
        texture_ptr = rhs.texture_ptr->clone();
    }

    return *this;
}
SV_Lambertian* SV_Lambertian::clone(void) const {
    return (new SV_Lambertian(*this));
}

Vector3D SV_Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const{
    return k_d * texture_ptr->get_color(sr) * INV_PI;
}
Vector3D SV_Lambertian::rho(const ShadeRec& sr, Vector3D& wo) const{
    return k_d * texture_ptr->get_color(sr);
}
