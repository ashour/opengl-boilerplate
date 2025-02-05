#include "labs/lab.h"
#include "system/window.h"
#include <memory>

namespace eo
{

class App
{
  public:
    void run();

  private:
    std::unique_ptr<Window> _window;
    std::unique_ptr<Lab> _lab;

    void loop();
};
} // namespace eo
