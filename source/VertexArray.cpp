#include "../include/VertexArray.h"

VertexArray::VertexArray(){
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}
VertexArray::~VertexArray(){
    unbind();
    glDeleteVertexArrays(1, &id);
}

void VertexArray::bind(){
    glBindVertexArray(id);
}
void VertexArray::unbind(){
    glBindVertexArray(0);
}

void VertexArray::setAttribPointer(int location, int size, GLsizei stride, void *offset){
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(location);
}
