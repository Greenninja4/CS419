#ifndef __CONSTANTS__
#define __CONSTANTS__

#include "Vector3D.h"
#include <stdlib.h> // RAND_MAX

#define BLACK Vector3D(0,0,0)
#define WHITE Vector3D(1, 1, 1)
#define RED Vector3D(1, 0, 0)
#define GREEN Vector3D(0, 1, 0)
#define BLUE Vector3D(0, 0, 1)
#define CYAN Vector3D(0, 1, 1)
#define MAGENTA Vector3D(0, 1, 1)
#define YELLOW Vector3D(1, 1, 0)

#define KEPSILON 0.0000000001
#define KHUGEVALUE 99999999999999

#define PI 3.1415926535897
#define INV_PI 0.31830988618

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


const float invRAND_MAX = 1.0 / (float)RAND_MAX;

#endif