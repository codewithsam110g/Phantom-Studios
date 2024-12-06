#define STB_IMAGE_IMPLEMENTATION
#include "../include/Texture.h"
Texture::Texture(std::string path, uint32_t texSlot, TextureType type){
    texType = type;
    glGenTextures(1, &tex);
    glActiveTexture(texSlot); //Optional for single Texture
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        throw std::runtime_error("Failed to Load Texture");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(){
    glActiveTexture(texSlot);
    glBindTexture(GL_TEXTURE_2D, tex);
}
void Texture::unbind(){
    glActiveTexture(texSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t Texture::getTexSlot(){return texSlot - GL_TEXTURE0;}

Texture::~Texture(){
    glDeleteTextures(1, &tex);
}
