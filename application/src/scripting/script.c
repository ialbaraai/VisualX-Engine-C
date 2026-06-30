#include "../../include/scripting/script.h"

class_t Script_Class = {0};

int script_constructor(script_t* instance, int argc, void** argv)
{
    if (instance)
    {
        return 1;
    }

    return 0;
}
void script_destructor(script_t* script)
{
    core_object_destroy(script);
}

void Script_Class_Init()
{
    core_class_init(&Script_Class, NULL, "ScriptClass", 0, 0, NULL, NULL, script_constructor, script_destructor);
}
void Script_Class_Destroy()
{
    core_class_destroy(&Script_Class);
}