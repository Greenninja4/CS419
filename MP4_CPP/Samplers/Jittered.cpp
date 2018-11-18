#include "Jittered.h"
#include "Maths.h"

// Big 6
Jittered::Jittered(void){}
Jittered::Jittered(const int& num_samples, const int& num_sets):
    Sampler(num_samples, num_sets) {
    generate_samples();
}
Jittered::Jittered(const Jittered& jittered):
    Sampler(jittered) {}
Jittered& Jittered::operator= (const Jittered& rhs){
    if (this == &rhs){
        return *this;
    }
    Sampler::operator=(rhs);
    return *this;
}
Jittered::~Jittered(void){}
Sampler* Jittered::clone() const{
    return (new Jittered(*this));
}

void Jittered::generate_samples(void){
    int n = (int) sqrt(num_samples);
    for (int p = 0; p < num_sets; p++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                Vector3D sample_point( (k+rand_float())/n, (j+rand_float())/n, 0);
                samples.push_back(sample_point);
            }
        }
    }
}

