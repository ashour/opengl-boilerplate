#include "object.h"
#include <cstdlib>

// clang-format off
inline const Object<8 * 6, 6 * 6> cube{
    .vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 0
        0.5f, -0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 1
        0.5f,  0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 2
        -0.5f,  0.5f,  0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 3
        // Back face
        -0.5f, -0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 4
        0.5f, -0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 5
        0.5f,  0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, // 6
        -0.5f,  0.5f, -0.5f,  (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX  // 7

    },
    .indices = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Right
        1, 5, 6,
        6, 2, 1,
        // Back
        7, 6, 5,
        5, 4, 7,
        // Left
        4, 0, 3,
        3, 7, 4,
        // Top
        3, 2, 6,
        6, 7, 3,
        // Bottom
        4, 5, 1,
        1, 0, 4
    }
};
// clang-format on
