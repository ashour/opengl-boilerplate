#pragma once

namespace eo
{
class Framebuffer
{
  public:
    Framebuffer(const int buffer_width, const int buffer_height);
    ~Framebuffer();

    void bind();
    void unbind();

    void bind_color_texture();

  private:
    unsigned int _fbo;
    unsigned int _rbo;
    unsigned int _tex_color_buffer;
};
} // namespace eo
