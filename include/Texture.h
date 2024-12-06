#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "stb_image.h"

#include <stdexcept>
#include <string>

enum TextureType{
    DIFFUSE = 0,
    SPECULAR,
    NORMAL,
    ROUGHNESS,
    AMBIENT_OCCLUSION
};

class Texture{
private:
    uint32_t tex;
    uint32_t texSlot;
    TextureType texType;
    Texture();
public:
    Texture(std::string path, uint32_t texSlot = GL_TEXTURE0, TextureType type = DIFFUSE);

    void bind();
    void unbind();

    uint32_t getTexSlot();
    TextureType getTexType(){return texType;}
    ~Texture();
};

#endif
