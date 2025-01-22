#pragma once

#include <iostream> // IWYU pragma: keep

#define EO_LOG_INFO(x) std::cout << "[INFO] " << x << std::endl

// Log header
#define EO_LOG_HEADING(x) std::cout << std::endl << "[INFO] === " << x << " ===" << std::endl

#define EO_LOG_ERROR(x) std::cerr << "[ERROR] " << x << std::endl
