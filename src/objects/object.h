#pragma once

#include "rendering/vertex.h"
#include <vector>

namespace eo
{
struct Object
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
} // namespace eo
