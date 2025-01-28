#pragma once

#include <glm/glm.hpp>

// Normalize color value
#define EO_NCOLV(x) static_cast<float>(x) / 255.0

#define EO_COLOR3(r, g, b) glm::vec3(EO_NCOLV(r), EO_NCOLV(g), EO_NCOLV(b))

#define EO_COLOR3_HEX(hex)                                                                         \
    glm::vec3(EO_NCOLV((hex >> 16) & 0xFF), EO_NCOLV((hex >> 8) & 0xFF), EO_NCOLV((hex) & 0xFF))
