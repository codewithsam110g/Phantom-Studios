#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "stb_image.h"

#include <stdexcept>
#include <string>

class Texture{
private:
    uint32_t tex;
    uint32_t texSlot;
    Texture();
public:
    Texture(std::string path, uint32_t texSlot = GL_TEXTURE0);

    void bind();
    void unbind();

    ~Texture();
};

#endif
