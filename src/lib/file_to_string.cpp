#include "file_to_string.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

namespace eo
{
std::string file_to_string(const std::string& filepath)
{
    std::ifstream file{filepath};
    if (!file.is_open())
    {
        std::cerr << "Could not open file (" << filepath << ")" << std::endl;
        return {};
    }

    return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
}
} // namespace eo
