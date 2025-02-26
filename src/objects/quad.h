#pragma once

#include "object.h"

namespace eo
{
// clang-format off
inline const Object quad{
    .vertices {
        {{ -0.5f, 0.0f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{  0.5f, 0.0f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
        {{ -0.5f, 0.0f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},
        {{  0.5f, 0.0f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},
    },
    .indices {
        0, 1, 2,
        2, 1, 3
    }
};
// clang-format on
} // namespace eo
