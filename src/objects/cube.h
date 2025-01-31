#include "object.h"

namespace eo
{
// clang-format off
inline const Object cube{
    .vertices{
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    },
    .indices{
         // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7 ,4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    },
    .attributes {
        // position
        {0, 3, ObjectAttrType::FLOAT, false, 8 * sizeof(float), (void*)0},
        // normal
        {1, 3, ObjectAttrType::FLOAT, true, 8 * sizeof(float), (void*)(3 * sizeof(float))},
        // uv
        {2, 2, ObjectAttrType::FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))}
    }
};
// clang-format on
} // namespace eo
