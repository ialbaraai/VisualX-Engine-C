#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <core.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

const char* vrtxShdrSrc = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "void main()\n"
                           "{\n"
                           "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                           "}\n";
const char* frgmntShdrRsc = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);\n"
                            "}\n";

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "VisualX Engine", NULL, NULL);

    if (!window)
    {
        printf("glfwCreateWindow Error!\n");

        glfwTerminate();

        return 1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        printf("gladLoadGL Error!\n");

        glfwDestroyWindow(window);
        glfwTerminate();

        return 2;
    }

    GLfloat vertices[] = {
        -0.5f, -0.5f * (float)sqrt(3) / 3.0f, 0.0f,
        0.5f, -0.5f * (float)sqrt(3) / 3.0f, 0.0f,
        0.0f, 0.5f * (float)sqrt(3) * 2 / 3.0f, 0.0f,
        -0.5f / 2.0f, 0.5f * (float)sqrt(3) / 6.0f, 0.0f,
        0.5f / 2.0f, 0.5f * (float)sqrt(3) / 6.0f, 0.0f,
        0.0f, -0.5f * (float)sqrt(3) / 3.0f, 0.0f
    };

    GLuint indices[] = {
        0, 3, 5,
        3, 2, 4,
        5, 3, 1
    };

    glViewport(0, 0, 800, 600);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vrtxShdrSrc, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frgmntShdrRsc, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}