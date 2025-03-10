#include "frame_buffer.h"
#include "rendering.h"

namespace eo
{

Framebuffer::Framebuffer(const int buffer_width, const int buffer_height)
{
    gldc(glGenFramebuffers(1, &_fbo));
    gldc(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));

    gldc(glGenTextures(1, &_tex_color_buffer));
    gldc(glBindTexture(GL_TEXTURE_2D, _tex_color_buffer));
    gldc(glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, buffer_width, buffer_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gldc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gldc(glBindTexture(GL_TEXTURE_2D, 0));

    gldc(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex_color_buffer, 0));

    gldc(glGenRenderbuffers(1, &_rbo));
    gldc(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
    gldc(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buffer_width, buffer_height));
    gldc(glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EO_LOG_ERROR("[Framebuffer Error] Framebuffer is not complete!");
    }
}

Framebuffer::~Framebuffer()
{
    gldc(glDeleteFramebuffers(1, &_fbo));
    gldc(glDeleteRenderbuffers(1, &_rbo));
    gldc(glDeleteTextures(1, &_tex_color_buffer));
}

void Framebuffer::bind() { gldc(glBindFramebuffer(GL_FRAMEBUFFER, _fbo)); }

void Framebuffer::unbind() { gldc(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

void Framebuffer::bind_color_texture() { gldc(glBindTexture(GL_TEXTURE_2D, _tex_color_buffer)); }

} // namespace eo
