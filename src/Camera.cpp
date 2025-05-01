#include "Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

#include <glm/glm.hpp>
Camera::Camera(Shader &shader, const uint32_t width, const uint32_t height,
               const float fov)
    : _shader(shader), _width(width), _height(height), _fov(fov) {
    printf("Initializing camera\n");

    // Initialize model matrix
    _model = glm::mat4(1.0f);
    _model =
        glm::rotate(_model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Initialize view matrix
    _view = glm::mat4(1.0f);
    _view = glm::translate(_view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Initialize projection matrix
    _projection = glm::perspective(glm::radians(45.0f),
                                   (float)width / (float)height, 0.1f, 100.0f);

    _shader.setMat4("model", _model);
}

Camera::~Camera() {}

void Camera::pan(float yaw, float pitch) {
    _cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    _cameraDirection.y = sin(glm::radians(pitch));
    _cameraFront = glm::normalize(_cameraDirection);
}
void Camera::update() {
    _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _globalUp);
    _projection = glm::perspective(glm::radians(_fov),
                                   (float)_width / (float)_height, 0.1f, 100.f);
    _shader.setMat4("view", _view);
    _shader.setMat4("projection", _projection);
}
