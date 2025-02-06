#include "cube.h"
#include "object.h"
#include "primitive.h"
#include "quad.h"

namespace eo
{
Object Primitive::cube() { return eo::cube; }
Object Primitive::quad() { return eo::quad; }
} // namespace eo
