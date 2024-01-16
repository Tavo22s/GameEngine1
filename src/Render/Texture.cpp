#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"
#include <glad/glad.h>  
#include <stb/stb_image.h>

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

void Texture::LoadTexture(std::string path)
{
    texture_path = path;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &channel, 0);
    std::cout<<stbi_failure_reason()<<std::endl;
    if(data)
    {
        if(data[0] == '\0')
            std::cout<<123<<std::endl;
        GLenum format;
        if(channel == 1)
            format = GL_RED;
        else if(channel == 3)
            format = GL_RGB;
        else if(channel == 4)
            format = GL_RGBA;
        else
            std::cout<<"Error format image"<<std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
    }
    else
        std::cout<<"Error to load image"<<std::endl;
    int idx = path.find_last_of('\\');
    if(idx != -1)
        textureName = Utils::Ex(path, idx+1, path.length());
    else
        textureName = path;
    stbi_image_free(data);
}