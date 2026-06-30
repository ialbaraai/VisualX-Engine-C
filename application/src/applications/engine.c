#include "../../include/applications/engine.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "stb_image.h"

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "VisualX Engine", NULL, NULL);

    if (!window)
    {
        printf("glfwCreateWindow error.\n");

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        printf("gladLoadGL error.\n");

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, onScreenResize);

    float vertices[] = {
        // positions // colors // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    float textureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* texturePath = "../images/wall.jpg";

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Couldn't load texture: %s.\n", texturePath);
    }

    stbi_image_free(data);

    shader_t shader = {0};
    Shader_Class.constructor(&shader, 2, (void*[]){"../shaders/vertex.glsl", "../shaders/fragment.glsl"});

    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        core_object_call(&shader, "use", 0, NULL);
        core_object_call(&shader, "setFloat", 2, (void*[]){"ourColor", &(float){1.0f}});

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
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

    glfwDestroyWindow(window);

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