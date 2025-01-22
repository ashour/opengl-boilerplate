#pragma once

namespace eo
{
enum class ObjectAttrType
{
    FLOAT
};

struct ObjectAttr
{
    const unsigned int index;
    const unsigned int size;
    const ObjectAttrType type;
    const bool is_normalized;
    const unsigned int stride;
    const void* pointer;
};
} // namespace eo
