
#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "glm/ext/matrix_float4x4.hpp"
#include <glad/glad.h>
#include <string>

class Shader {

public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, glm::mat4 &mat);
};

#endif
