#include "../../../include/backend/scene/scene.h"

#include "../../../include/backend/rendering/shader.h"

class_t Scene_Class = {0};

int scene_constructor(scene_t* instance, int argc, void** argv)
{
    if (instance)
    {
        if (argc == 1)
        {
            const char* file_path = argv[0];

            return 1;
        }
    }

    return 0;
}
void scene_destructor(scene_t* instance)
{
    core_object_destroy(instance);
}

void Scene_Class_Init()
{
    core_class_init(&Scene_Class, NULL, "SceneClass", 0, 0, NULL, NULL, scene_constructor, scene_destructor);
}
void Scene_Class_Destroy()
{
    core_class_destroy(&Scene_Class);
}