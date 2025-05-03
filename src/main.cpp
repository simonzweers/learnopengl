#include "Camera.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

uint32_t SCREEN_SIZE_X = 800;
uint32_t SCREEN_SIZE_Y = 600;

const float cameraSpeed = 0.5f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 globalUp = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraDirection;

float yaw = -90.0f;
float pitch = 0;

float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = (float)SCREEN_SIZE_X / 2, lastY = (float)SCREEN_SIZE_Y / 2;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    SCREEN_SIZE_X = width;
    SCREEN_SIZE_Y = height;
    glViewport(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    float lastX = 0, lastY = 0;
    float yaw = -90.0f;
    float pitch = 0;

    float xoffset = xpos - lastX;
    float yoffset = -(ypos - lastY);
    lastX = xpos;
    lastY = ypos;

    const float sens = 0.1f;
    xoffset *= sens;
    yoffset *= sens;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->pan(yaw, pitch);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera->zoom(yoffset);
}

void processInput(GLFWwindow *window) {

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->move(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->move(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->move(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->move(Camera::RIGHT, deltaTime);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_SIZE_X, SCREEN_SIZE_Y,
                                          "LearnOpenGL", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float vertices[][3] = {
        {-0.5f, -0.5f, 0.0f}, // bottom left,
        {0.5f, -0.5f, 0.0f},  // bottom right,
        {-0.5f, 0.5f, 0.0f},  // top left
        {0.5f, 0.5f, 0.0f},   // top right
    };

    float colors[][3] = {
        {1.0f, 0.0f, 0.0f}, // bottom left,
        {0.0f, 1.0f, 0.0f}, // bottom right,
        {0.0f, 0.0f, 1.0f}, // top left
        {1.0f, 0.0f, 1.0f}, // top right
    };

    float texCoords[] = {
        0.0f, 0.0f, // Lower left
        1.0f, 0.0f, // Lower right
        0.0f, 1.0f, // Top left
        1.0f, 1.0f, // Top right
    };

    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};

    unsigned int indecies[] = {
        // First triangle
        0,
        1,
        2,
        // Second triangle
        1,
        2,
        3,
    };
    float verticesCube[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f  //
    };
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *image_data =
        stbi_load("res/container.jpg", &width, &height, &nrChannels, 0);
    if (image_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::printf("Failed to load Texture");
    }
    stbi_image_free(image_data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image_data =
        stbi_load("res/awesomeface.png", &width, &height, &nrChannels, 0);
    if (image_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::printf("Failed to load Texture");
    }
    stbi_image_free(image_data);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube,
                 GL_STATIC_DRAW);

    // Position Attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::printf("Max vertex attributes: %d\n", nrAttributes);

    Shader shader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    shader.use();
    // shader.setInt("texture1", 0); // or with shader class
    // shader.setInt("texture2", 1); // or with shader class
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"),
                0); // set it manually
    glUniform1i(glGetUniformLocation(shader.ID, "texture2"),
                1); // set it manually

    Camera cam{shader, SCREEN_SIZE_X, SCREEN_SIZE_Y, fov};
    glfwSetWindowUserPointer(window, &cam);

    // // Create model matrix (Object scaling, rotations and translations)
    // glm::mat4 model = glm::mat4(1.0f);
    // model =
    //     glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f,
    //     0.0f));
    //
    // // Create View matrix
    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //
    // // Create projection matrix
    // glm::mat4 projection = glm::perspective(
    //     glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    //
    // shader.setMat4("model", model);
    // // shader.setMat4("view", view);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input handling
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0f));
        // trans =
        //     glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0,
        //     0.0, 1.0));
        // float scale = 0.5;
        // trans = glm::scale(trans, glm::vec3(scale, scale, scale));

        // glUniformMatrix4fv(, 1, GL_FALSE, );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // // const float radius = 10.0f;
        // // float camX = sin(glfwGetTime() * 0.1f) * radius;
        // // float camZ = cos(glfwGetTime() * 0.1f) * radius;
        // view = glm::lookAt(cameraPos,               //
        //                    cameraPos + cameraFront, //
        //                    globalUp);
        //
        // projection = glm::perspective(
        //     glm::radians(fov), (float)SCREEN_SIZE_X / (float)SCREEN_SIZE_Y,
        //     0.1f, 100.f);
        // shader.setMat4("view", view);
        // shader.setMat4("projection", projection);

        cam.update();
        shader.use();
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Check abd call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
