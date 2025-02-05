#include "app.h"
#include "lib/log.h"

int main()
{
    EO_LOG_HEADING("Main Start");

    eo::App app;
    app.run();

    return 0;
}
