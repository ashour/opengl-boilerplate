#include "file_to_string.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

std::string file_to_string(const std::string& file_path)
{
    std::ifstream file{file_path};
    if (!file.is_open())
    {
        std::cerr << "Could not open file (" << file_path << ")" << std::endl;
        return {};
    }

    return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
}
