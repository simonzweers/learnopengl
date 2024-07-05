
#ifndef _SHADER_HPP_
#define _SHADER_HPP_

class Shader {

public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
};

#endif
