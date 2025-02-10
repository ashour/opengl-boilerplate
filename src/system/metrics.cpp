#include "metrics.h"
#include "rendering/rendering.h"
#include "system/time.h"

namespace eo
{
unsigned int Metrics::_drawn_vertex_count{0};

void Metrics::begin_fragment_count_query()
{
    gldc(glGenQueries(1, &_query_id_fragment_count));
    gldc(glBeginQuery(GL_SAMPLES_PASSED, _query_id_fragment_count));
}

unsigned int Metrics::end_fragment_count_query()
{
    gldc(glEndQuery(GL_SAMPLES_PASSED));
    unsigned int result;
    gldc(glGetQueryObjectuiv(_query_id_fragment_count, GL_QUERY_RESULT, &result));
    return result;
}

float Metrics::frames_per_second()
{
    auto delta_time = Time::delta_time();
    return (delta_time > 0.0f) ? (1.0f / delta_time) : 0.0f;
}

} // namespace eo
