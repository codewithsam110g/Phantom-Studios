#include "../include/Buffer.h"

Buffer::Buffer(uint32_t type, int size, void * data, uint32_t usage){
    this->type = type;
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, size, data, usage);
}
void Buffer::bind(){
    glBindBuffer(type, id);
}
void Buffer::unbind(){
    glBindBuffer(type, 0);
}

void Buffer::setData(void* data){
    glBufferSubData(type, 0, sizeof(data), data);
}

Buffer::~Buffer(){
    glDeleteBuffers(1, &id);
}
