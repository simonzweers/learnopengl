#include "Shader.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <math.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        std::printf("Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::printf("Failed to initialize GLAD\n");
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[][3] = {
        {-0.5f, -0.5f, 0.5f}, // bottom left,
        {0.5f, -0.5f, 0.0f},  // bottom right,
        // {-0.5f, 0.5f, 0.5f},  // top left
        {0.0f, 0.5f, 0.0f}, // rop right
    };

    float colors[][3] = {
        {1.0f, 0.0f, 0.0f}, // bottom left,
        {0.0f, 1.0f, 0.0f}, // bottom right,
        {0.0f, 0.0f, 1.0f}, // rop right
    };

    /*
       unsigned int indecies[][3] = {
    // First triangle
    {0, 1, 2},
    // Second triangle
    {1, 2, 3},
    };
    */

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VBO_c;
    glGenBuffers(1, &VBO_c);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position Attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Color attiribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)(0));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::printf("Max vertex attributes: %d\n", nrAttributes);

    Shader shader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    shader.use();

    while (!glfwWindowShouldClose(window)) {
        // Input handling
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set color
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        float blueValue =
            sin(timeValue + (0.33333f * 2.0f * 3.1415f)) / 2.0f + 0.5f;
        float redValue =
            sin(timeValue + (0.666666 * 2.0f * 3.1415f)) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        // Render bs
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Check abd call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
