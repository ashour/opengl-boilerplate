#pragma once

#include "object.h"

namespace eo
{
// clang-format off
inline const Object plane{
    .vertices {
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    },
    .indices {
        0, 1, 2,
        2, 1, 3
    },
    .attributes {
        // position
        {0, 3, ObjectAttrType::FLOAT, false, 11 * sizeof(float), (void*)0},
        // normal
        {1, 3, ObjectAttrType::FLOAT, true, 11 * sizeof(float), (void*)(3 * sizeof(float))},
        // color
        {2, 3, ObjectAttrType::FLOAT, true, 11 * sizeof(float), (void*)(6 * sizeof(float))},
        // uv
        {3, 2, ObjectAttrType::FLOAT, false, 11 * sizeof(float), (void*)(9 * sizeof(float))}
    }
};
// clang-format on
} // namespace eo
