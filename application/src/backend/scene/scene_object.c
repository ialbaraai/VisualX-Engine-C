#include "../../../include/backend/scene/scene_object.h"

class_t Scene_Object_Class = {0};

int scene_object_constructor(scene_object_t* instance, int argc, void** argv)
{
    if (instance)
    {
        return 1;
    }

    return 0;
}
void scene_object_destructor(scene_object_t* instance)
{
    core_object_destroy(instance);
}

void Scene_Object_Class_Init()
{
    core_class_init(&Scene_Object_Class, NULL, "SceneObjectClass", 0, 0, NULL, NULL, scene_object_constructor, scene_object_destructor);
}
void Scene_Object_Class_Destroy()
{
    core_class_destroy(&Scene_Object_Class);
}