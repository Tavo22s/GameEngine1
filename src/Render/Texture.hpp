#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture
{
    public:
        ~Texture();

        void Render();

        inline void ShowDialogConfig() { showWindow = true; }
        inline unsigned int GetID() const   {   return textureID;   }
        inline std::string GetTexturePath() const { return texture_path; }
        inline std::string GetTextureName() const { return textureName; }
        inline int GetTextureWidth() const { return width; }
        inline int GetTextureHeight() const { return height; }

        std::string type;
        std::string name;

        friend Texture *LoadTexture(const std::string &path);
        friend Texture *LoadVoidTexture(unsigned int width, unsigned int height);
    private:
        unsigned int textureID;
        unsigned int wrap_s;
        unsigned int wrap_t;
        unsigned int filter_min;
        unsigned int filter_mag;

        std::string texture_path;
        std::string textureName;
        int width;
        int height;
        int channel;
        bool showWindow;

        Texture();
};

Texture *LoadTexture(const std::string &path);
Texture *LoadVoidTexture(unsigned int width, unsigned int height);

#endif // !TEXTURE_H