#pragma once

#include <fmt/color.h>
#include <fmt/core.h>

#define EO_LOG_INFO(format, ...)                                                                   \
    fmt::print(fmt::fg(fmt::terminal_color::blue), "[INFO] " format "\n", ##__VA_ARGS__)

#define EO_LOG_HEADING(format, ...)                                                                \
    fmt::print(fmt::fg(fmt::terminal_color::blue) | fmt::emphasis::bold,                           \
               "[INFO] " format "\n",                                                              \
               ##__VA_ARGS__)

#define EO_LOG_ERROR(format, ...)                                                                  \
    fmt::print(fmt::fg(fmt::terminal_color::red), "[ERROR] " format "\n", ##__VA_ARGS__)
