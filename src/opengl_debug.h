#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak // MSVC
#else
#define DEBUG_BREAK __builtin_trap // Clang/GCC
#endif

#define ASSERT(x)                                                              \
    if (!(x))                                                                  \
        DEBUG_BREAK();

// OpenGL debug call
#if DEBUG
#define gldc(x)                                                                \
    _gl_clear_error();                                                         \
    x;                                                                         \
    ASSERT(_gl_log_call(#x, __FILE__, __LINE__))
#else
#define gldc(x) x
#endif

static void _gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

// See https://www.khronos.org/opengl/wiki/OpenGL_Error#Testing_for_errors
static std::string _gl_error_code_to_string(const GLenum errorCode)
{
    switch (errorCode)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
        return "UNKNOWN_ERROR";
    }
}

static bool _gl_log_call(const char *function, const char *file, int line)
{
    while (GLenum errorCode = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << _gl_error_code_to_string(errorCode)
                  << ") " << function << " " << file << ":" << line
                  << std::endl;
        return false;
    }
    return true;
}
