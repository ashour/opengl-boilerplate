#include "metrics.h"
#include "system/time.h"

namespace eo
{

float Metrics::frames_per_second()
{
    auto delta_time = Time::delta_time();
    return (delta_time > 0.0f) ? (1.0f / delta_time) : 0.0f;
}

} // namespace eo
