#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Shader{
private:
    uint32_t id;
    uint32_t vert, frag;
    Shader();
public:
    Shader(std::string vertexPath, std::string fragmentPath);

    void bind();
    void unbind();

    uint32_t getId(){return id;};

    //Uniforms
    void setMat4(std::string varName, glm::mat4 mat);
    void setInt(std::string varName, int value);
    ~Shader();
};

#endif
