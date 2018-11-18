#ifndef __SAMPLER__
#define __SAMPLER__

#include "Vector3D.h"
#include <vector>

class Sampler{
    public:
        // Variables
        int num_samples;                // Number of sample points in a set
        int num_sets;                   // Number of smaple sets stored
        std::vector<Vector3D> samples;       // Sample points on a unit square
        std::vector<int> shuffled_indices;   // Shuffled samples array indices
        unsigned long count;            // Current number of sample points used
        int jump;                       // Random index jump

        // Big 6
        Sampler(void);
        Sampler(int num_samples, int num_sets);
        Sampler(const Sampler& sampler);
        Sampler& operator= (const Sampler& rhs);
        virtual ~Sampler(void);
        virtual Sampler* clone() const = 0;

        // Functions
        virtual void generate_samples(void) = 0;    // Generate sample set
        void setup_shuffled_indices(void);          // Set up randomly shuffled indices
        void shuffle_samples(void);                 // Randomly shuffle the samples in each pattern
        Vector3D sample_unit_square(void);          // Get next sample on unit square
};

#endif