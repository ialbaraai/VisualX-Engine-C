#include "../../../include/backend/rendering/mesh.h"

class_t Mesh_Class = {0};

int mesh_constructor(mesh_t* mesh, int argc, void** argv)
{
    if (mesh)
    {
        return 1;
    }

    return 0;
}
void mesh_destructor(mesh_t* mesh)
{
    core_object_destroy(mesh);
}

void Mesh_Class_Init()
{
    core_class_init(&Mesh_Class, NULL, "MeshClass", 0, 0, NULL, NULL, mesh_constructor, mesh_destructor);
}
void Mesh_Class_Destroy()
{
    core_class_destroy(&Mesh_Class);
}