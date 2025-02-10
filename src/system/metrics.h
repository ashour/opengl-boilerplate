#pragma once

namespace eo
{
class Metrics
{
  public:
    static void clear_drawn_vertex_count() { _drawn_vertex_count = 0; }
    static constexpr void add_to_drawn_vertex_count(const unsigned int number_to_add)
    {
        _drawn_vertex_count += number_to_add;
    }
    static unsigned int drawn_vertex_count() { return _drawn_vertex_count; }

    void begin_fragment_count_query();
    unsigned int end_fragment_count_query();

    static float frames_per_second();

  private:
    unsigned int _query_id_fragment_count;
    static unsigned int _drawn_vertex_count;
};
} // namespace eo
