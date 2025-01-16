#pragma once

#include <iostream>

#define LOG(x) std::cout << "[INFO] " << x << std::endl
#define LOG_T(x) std::cout << std::endl << "[INFO] " << x << std::endl
#define LOG_ERR(x) std::cerr << "[ERROR] " << x << std::endl
