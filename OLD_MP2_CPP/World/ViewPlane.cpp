#include "ViewPlane.h"

ViewPlane::ViewPlane(void): hres(400), vres(400), pixelSize(1.0), gamma(1.0), inv_gamma(1.0) {}
ViewPlane::ViewPlane(int hres, int vres, float pixelSize, float gamma): hres(hres), vres(vres), pixelSize(pixelSize), gamma(gamma) {inv_gamma = 1/gamma;}
ViewPlane::ViewPlane(const ViewPlane& vp): hres(vp.hres), vres(vp.vres), pixelSize(vp.pixelSize), gamma(vp.gamma), inv_gamma(vp.inv_gamma) {}
ViewPlane& ViewPlane::operator= (const ViewPlane& rhs){
    if (this == &rhs){
        return *this;
    }
    hres = rhs.hres;
    vres = rhs.vres;
    pixelSize = rhs.pixelSize;
    gamma = rhs.gamma;
    inv_gamma = rhs.inv_gamma;
    return *this;
}
ViewPlane::~ViewPlane(void){}

void ViewPlane::set_hres(const int h_res){
    this->hres = h_res;
}
void ViewPlane::set_vres(const int v_res){
    this->vres = v_res;
}
void ViewPlane::set_pixel_size(const float size){
    this->pixelSize = size;
}
void ViewPlane::set_gamma(const float g){
    this->gamma = g;
    this->inv_gamma = 1.0 / g;
}