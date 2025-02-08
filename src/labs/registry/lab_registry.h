#pragma once

#include "labs/lab.h"
#include "system/window.h"
#include <functional>
#include <map>
#include <string>

namespace eo
{
class LabRegistry
{
  public:
    using Creator = std::function<Lab*(Window&)>;

    static void register_lab(const std::string& name, Creator creator);
    static std::map<std::string, Creator>& labs() { return creators(); }
    static Lab* create(const std::string& name, Window& window);

  private:
    static std::map<std::string, Creator>& creators()
    {
        static std::map<std::string, Creator> creators;
        return creators;
    }
};
} // namespace eo
