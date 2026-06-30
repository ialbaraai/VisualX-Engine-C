#include "../../../include/backend/scene/light.h"

class_t Light_Class = {0};

int light_constructor(light_t* instance, int argc, void** argv)
{
    if (instance)
    {
        return 0;
    }

    return 1;
}
void light_destrcutor(light_t* instance)
{
    core_object_destroy(instance);
}

void Light_Class_Init()
{
    core_class_init(&Light_Class, NULL, "LightClass", 0, 0, NULL, NULL, light_constructor, light_destrcutor);
}
void Light_Class_Destroy()
{
    core_class_destroy(&Light_Class);
}