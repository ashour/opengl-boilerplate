#pragma once

#include <iostream> // IWYU pragma: keep

#define LOG(x) std::cout << "[INFO] " << x << std::endl

// Log header
#define LOG_H(x) std::cout << std::endl << "[INFO] === " << x << " ===" << std::endl

#define LOG_ERR(x) std::cerr << "[ERROR] " << x << std::endl
