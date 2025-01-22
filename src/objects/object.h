#pragma once

#include "object_attribute.h"
#include <vector>

namespace eo
{
struct Object
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<ObjectAttr> attributes;
};
} // namespace eo
