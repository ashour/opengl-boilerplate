#pragma once

#include "lib/color.h"
#include <glm/glm.hpp>
#include <string>

inline constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
inline constexpr unsigned int OPENGL_MINOR_VERSION = 1;

inline const std::string WINDOW_TITLE = "OpenGL Boilerplate";
inline constexpr unsigned int WINDOW_WIDTH = 1920;
inline constexpr unsigned int WINDOW_HEIGHT = 1080;

inline constexpr const char* VERTEX_SHADER_FILEPATH = "./resources/shaders/default.vert";
inline constexpr const char* FRAGMENT_SHADER_FILEPATH = "./resources/shaders/default.frag";

inline constexpr glm::vec4 SCENE_CLEAR_COLOR{NCOLV(179), NCOLV(195), NCOLV(239), 1.0};
