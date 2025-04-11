#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Shader.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cstdint>

class Camera {
public:
    Camera(Shader &shader, const uint32_t width, const uint32_t height,
           const float fov);
    Camera(Camera &&) = delete;
    Camera(const Camera &) = delete;
    Camera &operator=(Camera &&) = delete;
    Camera &operator=(const Camera &) = delete;
    ~Camera();

    void move();
    void pan();

    void update();

private:
    Shader &_shader;

    uint32_t _width;
    uint32_t _height;
    float _fov;
    glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0, 3.0f);
    glm::vec3 _cameraFront = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 _cameraUp = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 _globalUp = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 _cameraDirection;

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};

#endif // _CAMERA_HPP_
