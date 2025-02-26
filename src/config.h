#pragma once

#include "lib/color.h"
#include <glm/glm.hpp>
#include <string>

inline constexpr unsigned int OPENGL_MAJOR_VERSION = 4;
inline constexpr unsigned int OPENGL_MINOR_VERSION = 1;

inline const std::string WINDOW_TITLE = "OpenGL Boilerplate";
inline constexpr unsigned int WINDOW_WIDTH = 1920 / 2;
inline constexpr unsigned int WINDOW_HEIGHT = 1080 / 2;

inline const std::string SHADER_DIR = "./resources/shaders/";
inline const std::string DEFAULT_VERTEX_SHADER_FILEPATH = SHADER_DIR + "phong.vert";
inline const std::string DEFAULT_FRAGMENT_SHADER_FILEPATH = SHADER_DIR + "phong.frag";

inline const std::string TEXTURE_DIR = "./resources/textures/";

inline constexpr glm::vec4 SCENE_CLEAR_COLOR{EO_NCOLV(1), EO_NCOLV(17), EO_NCOLV(29), 1.0};

inline constexpr bool SHOW_UI_DEMO = false;
inline constexpr bool SHOW_UI_METRICS = true;
