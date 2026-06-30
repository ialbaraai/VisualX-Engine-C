#include "../../include/engine/visualx.h"

#include "../../include/applications/engine.h"

#include "../../include/scripting/script.h"
#include "../../include/backend/scene/scene_object.h"
#include "../../include/backend/parser/parser.h"
#include "../../include/backend/scene/scene.h"
#include "../../include/backend/rendering/shader.h"
#include "../../include/backend/rendering/mesh.h"
#include "../../include/backend/rendering/texture.h"
#include "../../include/backend/scene/light.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void init_system();
void destroy_system();

void run()
{
    init_system();

    engine_t engine = {0};
    engine_init(&engine);
    engine_run(&engine);
    engine_destroy(&engine);

    destroy_system();
}

void init_system()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Scene_Object_Class_Init();
    
    Shader_Class_Init();

    Script_Class_Init();

    Parser_Class_Init();

    Mesh_Class_Init();

    Texture_Class_Init();

    Light_Class_Init();

    Scene_Class_Init();
}
void destroy_system()
{
    Scene_Class_Destroy();

    Light_Class_Destroy();

    Texture_Class_Destroy();

    Mesh_Class_Destroy();

    Parser_Class_Destroy();

    Script_Class_Destroy();

    Shader_Class_Destroy();

    Scene_Object_Class_Destroy();

    glfwTerminate();
}