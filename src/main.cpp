#include "app.h"
#include "lib/log.h"

int main()
{
    LOG_H("Main Start");

    eo::App app;
    return app.run();
}
