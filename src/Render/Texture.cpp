#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"
#include <glad/glad.h>  
#include "stb_image.h"

#include "../Engine/Utils.hpp"

Texture::Texture()
{
    filter_min = GL_LINEAR;
    filter_mag = GL_LINEAR;
    wrap_s = GL_REPEAT;
    wrap_t = GL_REPEAT;

    texture_path = "";
    textureName = "";
    textureID = 0;
    showWindow = false;
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

void Texture::Render()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture *LoadTexture(const std::string &path)
{
    Texture *texture = new Texture();
    texture->texture_path = path;
    #ifdef __linux__
        Utils::FixPathToLinux(texture->texture_path);
    #endif
    glGenTextures(1, &texture->textureID);
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    std::cout<<"Loading image from: "<<texture->texture_path<<std::endl;
    texture->name = path;
    unsigned char *data = stbi_load(texture->texture_path.c_str(), &texture->width, &texture->height, &texture->channel, 0);
    if(data)
    {
        GLenum format;
        if(texture->channel == 1)
            format = GL_RED;
        else if(texture->channel == 3)
            format = GL_RGB;
        else if(texture->channel == 4)
            format = GL_RGBA;
        else
            std::cout<<"Error format image"<<std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrap_s);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filter_mag);
    }
    else
    {
        std::cout<<stbi_failure_reason()<<std::endl;
        std::cout<<"Error to load image"<<std::endl;
    }
    #ifdef __linux__
        int idx = path.find_last_of('/') + 1;
    #else
        int idx = path.find_last_of('\\') + 1;
    #endif
    if(idx != -1)
        texture->textureName = Utils::Ex(path, idx+1, path.length());
    else
        texture->textureName = path;
    stbi_image_free(data);

    return texture;
}