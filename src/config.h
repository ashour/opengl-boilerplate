#pragma once

#include "lib/color.h"
#include <glm/glm.hpp>
#include <string>

inline constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
inline constexpr unsigned int OPENGL_MINOR_VERSION = 1;

inline const std::string WINDOW_TITLE = "OpenGL Boilerplate";
inline constexpr unsigned int WINDOW_WIDTH = 960;
inline constexpr unsigned int WINDOW_HEIGHT = 640;

inline const std::string SHADER_DIR = "./resources/shaders/";
inline const std::string DEFAULT_VERTEX_SHADER_FILEPATH = SHADER_DIR + "phong.vert";
inline const std::string DEFAULT_FRAGMENT_SHADER_FILEPATH = SHADER_DIR + "phong.frag";

inline const std::string TEXTURE_DIR = "./resources/textures/";

inline constexpr glm::vec4 SCENE_CLEAR_COLOR{NCOLV(179), NCOLV(195), NCOLV(239), 1.0};
