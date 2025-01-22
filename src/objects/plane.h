#pragma once

#include "object.h"

namespace eo
{
// clang-format off
inline const Object plane{
    .vertices {
        -100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 0.0f,  50.0f,
         100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 50.0f, 50.0f,
        -100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f, 50.0f,  0.0f,
    },
    .indices {
        0, 1, 2,
        2, 1, 3
    },
    .attributes {
        {0, 3, ObjectAttrType::FLOAT, false, 8 * sizeof(float), (void*)0},
        {1, 3, ObjectAttrType::FLOAT, true, 8 * sizeof(float), (void*)(3 * sizeof(float))},
        {2, 2, ObjectAttrType::FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))}
    }
};
// clang-format on
} // namespace eo
