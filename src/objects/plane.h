#include "lib/color.h"
#include "object.h"

namespace eo
{
// clang-format off
inline const Object<4*6, 6> plane{
    .vertices = {
        -100.0f, 0.0f,  100.0f,  NCOLV(41.0), NCOLV(16.0), NCOLV(35), // 0
         100.0f, 0.0f,  100.0f,  NCOLV(41.0), NCOLV(16.0), NCOLV(35), // 1
        -100.0f, 0.0f, -100.0f,  NCOLV(41.0), NCOLV(16.0), NCOLV(35), // 2
         100.0f, 0.0f, -100.0f,  NCOLV(41.0), NCOLV(16.0), NCOLV(35), // 3
    },
    .indices = {
        0, 1, 2,
        2, 1, 3
    }
};
// clang-format on
} // namespace eo
