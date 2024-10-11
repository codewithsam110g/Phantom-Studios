#ifndef BUFFER_H
#define BUFFER_H
#include "glad/glad.h"
class Buffer{
private:
    uint32_t id;
    uint32_t type;
    Buffer(){};
public:
    Buffer(uint32_t type, int size, void * data, uint32_t usage = GL_STATIC_DRAW);

    void bind();
    void unbind();

    void setData(void* data);

    ~Buffer();
};

#endif
