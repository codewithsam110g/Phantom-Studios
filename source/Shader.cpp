#include "../include/Shader.h"
#include "../include/Utils.h"

#include <stdexcept>
#include <string>

Shader::Shader(std::string vertexPath, std::string fragmentPath){
   std::string vertCode, fragCode;

   vertCode = Utils::readFromFile(vertexPath.c_str());
   fragCode = Utils::readFromFile(fragmentPath.c_str());

   const char* vc = vertCode.c_str();
   const char* fc = fragCode.c_str();


   vert = glCreateShader(GL_VERTEX_SHADER);
   frag = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(vert, 1, &vc, nullptr);
   glShaderSource(frag, 1, &fc, nullptr);

   glCompileShader(vert);
   glCompileShader(frag);

   int success;
   char infoLog[512];
   glGetShaderiv(vert, GL_COMPILE_STATUS, &success);

   if (!success)
   {
   	glGetShaderInfoLog(vert, 512, nullptr, infoLog);
    throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
   }

   glGetShaderiv(frag, GL_COMPILE_STATUS, &success);

   if (!success)
   {
   	glGetShaderInfoLog(frag, 512, nullptr, infoLog);
    throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));

   }

   id = glCreateProgram();
   glAttachShader(id, vert);
   glAttachShader(id, frag);
   glLinkProgram(id);
   glGetProgramiv(id, GL_LINK_STATUS, &success);

   if (!success)
   {
   	glGetProgramInfoLog(id, 512, nullptr, infoLog);
    throw std::runtime_error("ERROR::SHADER::LINKING::FAILED\n" + std::string(infoLog));

   }

   glDeleteShader(vert);
   glDeleteShader(frag);

   glUseProgram(0);

}

void Shader::bind(){
    glUseProgram(id);
}
void Shader::unbind(){
    glUseProgram(0);
}

//Uniforms
void Shader::setMat4(std::string varName, glm::mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(id, varName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

Shader::~Shader(){
    glDetachShader(id, vert);
    glDetachShader(id, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    glDeleteProgram(id);
}
