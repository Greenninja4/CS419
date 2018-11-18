#include "Sampler.h"
#include "Maths.h"
#include <algorithm>
#include <iostream>


// Big 6
Sampler::Sampler(void):
    num_samples(1),
    num_sets(1){
    setup_shuffled_indices();
}
Sampler::Sampler(int num_samples, int num_sets):
    num_samples(num_samples),
    num_sets(num_sets){
    setup_shuffled_indices();
}
Sampler::Sampler(const Sampler& sampler):
    num_samples(sampler.num_samples),
    num_sets(sampler.num_sets),
    samples(sampler.samples),
    shuffled_indices(sampler.shuffled_indices),
    count(sampler.count),
    jump(sampler.jump){}
Sampler& Sampler::operator= (const Sampler& rhs){
    if (this == &rhs){
        return *this;
    }
    num_samples = rhs.num_samples;
    num_sets = rhs.num_sets;
    samples = rhs.samples;
    shuffled_indices = rhs.shuffled_indices;
    count = rhs.count;
    jump = rhs.jump;
    return *this;
}
Sampler::~Sampler(void){}

// Functions
void Sampler::setup_shuffled_indices(void){
    shuffled_indices.reserve(num_samples * num_sets);
    std::vector<int> indices;
    for (int j = 0; j < num_samples; j++){
        indices.push_back(j);
    }
    for (int p = 0; p < num_sets; p++){
        std::random_shuffle(indices.begin(), indices.end());
        for (int j = 0; j < num_samples; j++){
            shuffled_indices.push_back(indices[j]);
        }
    }
}

void Sampler::shuffle_samples(void){
    // Shuffle x
    for (int p = 0; p < num_sets; p++){
        for (int i = 0; i < num_samples; i++){
            int target = rand_int() % num_samples + p*num_samples;
            int temp = samples[i + p*num_samples + 1].x;
            samples[i + p*num_samples + 1].x = samples[target].x;
            samples[target].x = temp;
        }
    }
    // Shuffle y
    for (int p = 0; p < num_sets; p++){
        for (int i = 0; i < num_samples; i++){
            int target = rand_int() % num_samples + p*num_samples;
            int temp = samples[i + p*num_samples + 1].y;
            samples[i + p*num_samples + 1].y = samples[target].y;
            samples[target].y = temp;
        }
    }
}

Vector3D Sampler::sample_unit_square(void){
    if (count % num_samples == 0){
        jump = (rand_int() % num_sets) * num_samples;
    }
    return (samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}
