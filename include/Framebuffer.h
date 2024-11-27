#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad/glad.h"
#include <stdexcept>

class Framebuffer{
private:
    uint32_t fbo, texture, rbo;
    Framebuffer();
public:
    Framebuffer(int width, int height);

    uint32_t getTexture();

    void resize(int width, int height);
    
    void bind();
    void unbind();
};

#endif
