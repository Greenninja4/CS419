#ifndef __JITTERED__
#define __JITTERED__

#include "Sampler.h"

class Jittered : public Sampler{
    public:
        // Big 6
        Jittered(void);
        Jittered(const int& num_samples, const int& num_sets);
        Jittered(const Jittered& jittered);
        Jittered& operator= (const Jittered& rhs);
        virtual ~Jittered(void);
        virtual Sampler* clone() const;

        // Functions
        virtual void generate_samples(void);
};

#endif