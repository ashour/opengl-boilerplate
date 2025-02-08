#include "lab_registry.h"

namespace eo
{

void LabRegistry::register_lab(const std::string& name, Creator creator)
{
    creators()[name] = creator;
}

Lab* LabRegistry::create(const std::string& name, Window& window)
{
    auto it = creators().find(name);
    return (it != creators().end()) ? it->second(window) : nullptr;
}

} // namespace eo
