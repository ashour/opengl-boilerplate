#pragma once

#include <array>

template <size_t VertexIndexCount, size_t IndexIndexCount>
struct Object
{
    std::array<float, VertexIndexCount> vertices;
    std::array<unsigned int, IndexIndexCount> indices;
};
