#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glad/glad.h"

class VertexArray{
private:
    uint32_t id;
public:
    VertexArray();
    ~VertexArray();
    
    void bind();
    void unbind();
    
    void setAttribPointer(int location, int size, GLsizei stride, void *offset);
};

#endif
