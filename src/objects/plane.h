#include "object.h"

namespace eo
{
// clang-format off
inline const Object<4*8, 6> plane{
    .vertices = {
        -100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 0.0f,  10.0f,
         100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 10.0f, 10.0f,
        -100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f, 10.0f,  0.0f,
    },
    .indices = {
        0, 1, 2,
        2, 1, 3
    }
};
// clang-format on
} // namespace eo
