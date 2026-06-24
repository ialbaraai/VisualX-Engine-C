#include "../../include/applications/engine.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void onScreenResize(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

void engine_init(engine_t* engine)
{
    if (!engine)
        return;

    core_string_init_data(&engine->name, 32, "VisualX Engine");
}

int engine_run(engine_t* engine)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "VisualX Engine", NULL, NULL);

    if (!window)
    {
        printf("glfwCreateWindow error.\n");

        glfwTerminate();

        core_class_destroy(&Shader_Class);

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        printf("gladLoadGL error.\n");

        glfwDestroyWindow(window);
        glfwTerminate();

        core_class_destroy(&Shader_Class);

        return -1;
    }

    Shader_Class_Init();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, onScreenResize);

    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };

    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    object_t shader = {0};
    Shader_Class.constructor(&shader, 2, (void*[]){"../shaders/vertex.glsl", "../shaders/fragment.glsl"});

    float multiplier = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);
     
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        core_object_call(&shader, "use", 0, NULL);
        core_object_call(&shader, "setFloat", 2, (void*[]){"ourColor", &multiplier});

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //swap buffers and checking & calling events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Shader_Class.destructor(&shader);

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);

    glfwTerminate();

    core_class_destroy(&Shader_Class);

    return 0;
}

void engine_destroy(engine_t* engine)
{
    if (engine)
    {
        core_string_destroy(&engine->name);
    }
}

void onScreenResize(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}