#include "lib/color.h"
#include "object.h"

namespace eo
{
// clang-format off
inline const Object<8 * 6, 6 * 6> cube{
    .vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  NCOLV(42),  NCOLV(15),  NCOLV(83),  // 0
         0.5f, -0.5f,  0.5f,  NCOLV(171), NCOLV(32),  NCOLV(101), // 1
         0.5f,  0.5f,  0.5f,  NCOLV(255), NCOLV(79),  NCOLV(105), // 2
        -0.5f,  0.5f,  0.5f,  NCOLV(255), NCOLV(247), NCOLV(248), // 3
        // Back face
        -0.5f, -0.5f, -0.5f,  NCOLV(255), NCOLV(129), NCOLV(65),  // 4
         0.5f, -0.5f, -0.5f,  NCOLV(255), NCOLV(218), NCOLV(69),  // 5
         0.5f,  0.5f, -0.5f,  NCOLV(51),  NCOLV(104), NCOLV(220), // 6
        -0.5f,  0.5f, -0.5f,  NCOLV(73),  NCOLV(231), NCOLV(236),  // 7
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
} // namespace eo
