#include "include/applications/engine.h"

int main(void)
{
    engine_t engine = {0};
    engine_init(&engine);
    engine_run(&engine);
    engine_destroy(&engine);

    return 0;
}