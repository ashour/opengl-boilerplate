#include "labs/lab.h"
#include "system/ui.h"
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
    std::unique_ptr<UI> _ui;

    void loop();

    Lab* _lab;
    std::string _current_lab_name;
    void lab_selector();
};
} // namespace eo
