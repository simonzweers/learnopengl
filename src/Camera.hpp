#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Shader.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cstdint>

class Camera {
public:
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };
    Camera(const uint32_t width, const uint32_t height, const float fov);
    Camera(Camera &&) = delete;
    Camera(const Camera &) = delete;
    Camera &operator=(Camera &&) = delete;
    Camera &operator=(const Camera &) = delete;
    ~Camera();

    void move(Direction direction, float deltatime);
    void pan(float yaw, float pitch);
    void zoom(double fov);

    void update();

    void updateAspect(uint32_t width, uint32_t height);

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

    glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0, 3.0f);
    glm::vec3 _cameraFront = glm::vec3(0.0, 0.0, -1.0);

private:
    // Shader &_shader;

    const float _cameraSpeed = 0.5f;
    uint32_t _width;
    uint32_t _height;
    float _fov;
    float _aspect;
    glm::vec3 _cameraUp = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 _globalUp = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 _cameraDirection;
};

#endif // _CAMERA_HPP_
