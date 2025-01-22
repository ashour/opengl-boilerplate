#include "cube.h"
#include "object.h"
#include "plane.h"
#include "primitive.h"

namespace eo
{
Object Primitive::cube() { return eo::cube; }
Object Primitive::plane() { return eo::plane; }
} // namespace eo
