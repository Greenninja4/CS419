#ifndef __PERFECT_TRANSMITTER__
#define __PERFECT_TRANSMITTER__

#include "BTDF.h"
#include "Vector3D.h"
#include "ShadeRec.h"

class PerfectTransmitter : public BTDF {
    public:
        float kt;
        float ior;

        PerfectTransmitter(void);
        PerfectTransmitter(float kt, float ior);
        PerfectTransmitter(const PerfectTransmitter& perfect_transmitter);
        PerfectTransmitter& operator= (const PerfectTransmitter& rhs);
        virtual ~PerfectTransmitter(void);
        virtual PerfectTransmitter* clone(void) const;

        virtual Vector3D f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        virtual Vector3D sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
        virtual Vector3D rho(const ShadeRec& sr, const Vector3D& wo) const;
        virtual bool tir(const ShadeRec& sr) const;
};

#endif