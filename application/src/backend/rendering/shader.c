#include "../../../include/backend/rendering/shader.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <glad/gl.h>

class_t Shader_Class = {0};

void shader_class_use(object_t* object, int argc, void** argv)
{
    (void)argc;
    (void)argv;

    if (object)
        glUseProgram(*(unsigned int*)core_object_member_get(object, "ID")->value);
}
void shader_class_setbool(object_t* object, int argc, void** argv)
{
    if (object && argc == 2)
    {
        const char* name = argv[0];
        bool value = *(bool*)argv[1];
        glUniform1i(glGetUniformLocation(*(unsigned int*)core_object_member_get(object, "ID")->value, name), (int)value);
    }
}
void shader_class_setint(object_t* object, int argc, void** argv)
{
    if (object && argc == 2)
    {
        const char* name  = argv[0];
        int value = *(int*)argv[1];

        glUniform1i(glGetUniformLocation(*(unsigned int*)core_object_member_get(object, "ID")->value, name), value);
    }
}
void shader_class_setfloat(object_t* object, int argc, void** argv)
{
    if (object && argc == 2)
    {
        const char* name = argv[0];
        float value = *(float*)argv[1];

        glUniform1f(glGetUniformLocation(*(unsigned int*)core_object_member_get(object, "ID")->value, name), value);
    }
}

int shader_constructor(object_t* instance, int argc, void** argv)
{
    method_t useMethod = {0};
    core_method_init(&useMethod, "use", shader_class_use, PUBLIC);

    method_t setBoolMethod = {0};
    core_method_init(&setBoolMethod, "setBool", shader_class_setbool, PUBLIC);

    method_t setIntMethod = {0};
    core_method_init(&setIntMethod, "setInt", shader_class_setint, PUBLIC);

    method_t setFloatMethod = {0};
    core_method_init(&setFloatMethod, "setFloat", shader_class_setfloat, PUBLIC);

    member_t idMember = {0};
    core_member_init(&idMember, "ID", sizeof(unsigned int), PUBLIC, NULL, NULL);

    core_object_init(instance, &Shader_Class, "shader", 1, 4, &idMember, (method_t[]){useMethod, setBoolMethod, setIntMethod, setFloatMethod});

    core_member_destroy(&idMember);
    core_method_destroy(&setFloatMethod);
    core_method_destroy(&setIntMethod);
    core_method_destroy(&setBoolMethod);
    core_method_destroy(&useMethod);

    if (argc == 2)
    {
        const char* vertexPath = argv[0];
        const char* fragmentPath = argv[1];

        if (vertexPath && fragmentPath)
        {
            string_t vertexSource = {0};
            string_t fragmentSource = {0};

            core_string_init(&vertexSource, 128);
            core_string_init(&fragmentSource, 128);

            file_t vertexShaderFile = {0};
            file_t fragmentShaderFile = {0};

            core_file_init(&vertexShaderFile, vertexPath, 8, 64);
            core_file_init(&fragmentShaderFile, fragmentPath, 8, 64);

            core_file_read(&vertexShaderFile);
            core_file_read(&fragmentShaderFile);

            for (size_t i = 0; i < vertexShaderFile.content.size; ++i)
            {
                core_string_append_string(&vertexSource, (string_t*)core_vector_get(&vertexShaderFile.content, i));
                core_string_append_char(&vertexSource, '\n');
            }

            for (size_t i = 0; i < fragmentShaderFile.content.size; ++i)
            {
                core_string_append_string(&fragmentSource, (string_t*)core_vector_get(&fragmentShaderFile.content, i));
                core_string_append_char(&fragmentSource, '\n');
            }

            unsigned int vertex, fragment, id;
            int success;
            char infoLog[512];

            const char* vrtxSrc[] = { vertexSource.data };
            const char* frgmntSrc[] = { fragmentSource.data };

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, vrtxSrc, NULL);
            glCompileShader(vertex);

            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                printf("ERROR: VERTEX SHADER COMPILATION FAILED: %s.\n", infoLog);
            }

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, frgmntSrc, NULL);
            glCompileShader(fragment);

            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                printf("ERROR: FRAGMENT SHADER COMPILATION FAILED: %s.\n", infoLog);
            }

            id = glCreateProgram();
            glAttachShader(id, vertex);
            glAttachShader(id, fragment);
            glLinkProgram(id);

            glGetProgramiv(id, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(id, 512, NULL, infoLog);
                printf("ERROR: PROGRAM LINKING FAILED: %s.\n", infoLog);
            }

            core_member_set(core_object_member_get(instance, "ID"), &id);

            glDeleteShader(vertex);
            glDeleteShader(fragment);

            core_file_destroy(&vertexShaderFile);
            core_file_destroy(&fragmentShaderFile);

            core_string_destroy(&fragmentSource);
            core_string_destroy(&vertexSource);
        }
    }
    return 1;
}

void shader_destructor(object_t* instance)
{
    if (!instance) return;

    glDeleteProgram(*(unsigned int*)core_object_member_get(instance, "ID")->value);
    core_object_destroy(instance);
}

void Shader_Class_Init()
{
    core_class_init(&Shader_Class, NULL, "ShaderClass", 0, 0, NULL, NULL, shader_constructor, shader_destructor);
}
void Shader_Class_Destroy()
{
    core_class_destroy(&Shader_Class);
}