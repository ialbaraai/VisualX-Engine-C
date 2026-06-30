#include "../../../include/backend/rendering/texture.h"

class_t Texture_Class = {0};

int texture_constructor(texture_t* instance, int argc, void** argv)
{
    if (instance)
    {
        return 1;
    }

    return 0;
}
void texture_destructor(texture_t* texture)
{
    core_object_destroy(texture);
}

void Texture_Class_Init()
{
    core_class_init(&Texture_Class, NULL, "TextureClass", 0, 0, NULL, NULL, texture_constructor, texture_destructor);
}
void Texture_Class_Destroy()
{
    core_class_destroy(&Texture_Class);
}