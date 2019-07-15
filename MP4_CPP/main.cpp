#include <time.h>
#include <iostream>
#include <chrono>

#include "World.h"

int main(void){
    auto start_time = std::chrono::system_clock::now();

    World w;
    w.build();
    w.render_scene();

    //w.animate();

    std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start_time;
    double execution_time = elapsed_time.count();
    std::cout << execution_time << " seconds to render your image!" << std::endl;
    std::cout << w.num_rays << " rays used to render your image!" << std::endl;

    return 0;
}