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

    float vertices[][3] = {{-0.5f, -0.5f, 0.5f}, // bottom left,
                           {0.5f, -0.5f, 0.0f},  // bottom right,
                           {-0.5f, 0.5f, 0.5f},  // top left
                           {0.5f, 0.5f, 0.0f}};  // rop right
    unsigned int indecies[][3] = {
        // First triangle
        {0, 1, 2},
        // Second triangle
        {1, 2, 3},
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::printf("Max vertex attributes: %d\n", nrAttributes);

    // VERTEX SHADER
    FILE *fpVertexShader = fopen("shaders/vertex.glsl", "r");
    if (fpVertexShader == NULL) {
        perror("Could not open vertex.glsl: ");
        return EXIT_FAILURE;
    } else {
        std::printf("VERTEX SHADER: File openend\n");
    };
    fseek(fpVertexShader, 0, SEEK_END);
    long fpVertexShaderLen = ftell(fpVertexShader);
    rewind(fpVertexShader);
    char *vertexShaderSource = (char *)malloc(fpVertexShaderLen);
    fread(vertexShaderSource, 1, fpVertexShaderLen, fpVertexShader);
    printf("Program:\n%s\n", vertexShaderSource);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // FRAGMENT SHADER
    FILE *fpFragmentShader = fopen("shaders/fragment.glsl", "r");
    if (fpFragmentShader == NULL) {
        perror("Could not open fragment.glsl: ");
        return EXIT_FAILURE;
    } else {
        std::printf("FRAGMENT SHADER: File openend\n");
    };
    fseek(fpFragmentShader, 0, SEEK_END);
    long fpFragmentShaderLen = ftell(fpFragmentShader);
    rewind(fpFragmentShader);
    char *fragmentShaderSource = (char *)malloc(fpFragmentShaderLen);
    fread(fragmentShaderSource, 1, fpFragmentShaderLen, fpFragmentShader);
    printf("Program:\n%s\n", fragmentShaderSource);
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::printf("ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n%s\n",
                    infoLog);
    }

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    // Delete no longer needed shaders
    fclose(fpVertexShader);
    free(vertexShaderSource);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

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
        int vertexColorLocation =
            glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        // Render bs
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check abd call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
