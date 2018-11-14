
#include "PerfectTransmitter.h"
#include "math.h"
#include "Constants.h"

PerfectTransmitter::PerfectTransmitter(void):
    kt(0),
    ior(1) {}
PerfectTransmitter::PerfectTransmitter(float kt, float ior):
    kt(kt),
    ior(ior) {}
PerfectTransmitter::PerfectTransmitter(const PerfectTransmitter& perfect_transmitter):
    kt(perfect_transmitter.kt),
    ior(perfect_transmitter.ior) {}
PerfectTransmitter& PerfectTransmitter::operator= (const PerfectTransmitter& rhs){
    if (this == &rhs){
        return *this;
    }
    BTDF::operator= (rhs);
    kt = rhs.kt;
    ior = rhs.ior;
    return *this;
}
PerfectTransmitter::~PerfectTransmitter(void){}
PerfectTransmitter* PerfectTransmitter::clone(void) const{
    return (new PerfectTransmitter(*this));
}

Vector3D PerfectTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wt) const{
    Vector3D n(sr.normal);
    float cos_thetai = n * wo;
    float eta = ior;

    if (cos_thetai < 0.0){
        cos_thetai = -cos_thetai;
        n = -n;
        eta = 1.0/eta;
    }

    float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
    float cos_theta2 = sqrt(temp);
    Vector3D wt2 = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;
    return (kt / (eta * eta) * WHITE / fabs(sr.normal * wt2));
}
Vector3D PerfectTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const{
    Vector3D n(sr.normal);
    float cos_thetai = n * wo;
    float eta = ior;

    if (cos_thetai < 0.0){
        cos_thetai = -cos_thetai;
        n = -n;
        eta = 1.0/eta;
    }

    float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
    float cos_theta2 = sqrt(temp);
    wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;
    return (kt / (eta * eta) * WHITE / fabs(sr.normal * wt));
}
Vector3D PerfectTransmitter::rho(const ShadeRec& sr, const Vector3D& wo) const{
    return BLACK;
}
bool PerfectTransmitter::tir(const ShadeRec& sr) const{
    Vector3D wo(-sr.ray.d);
    float cos_thetai = sr.normal * wo;
    float eta = ior;

    if (cos_thetai < 0.0){
        eta = 1.0 / eta;
    }

    return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta*eta) < 0.0);
}
