#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture
{
    public:
        Texture();
        ~Texture();

        void Render();
        void LoadTexture(std::string path);

        inline void ShowDialogConfig() { showWindow = true; }
        inline unsigned int GetID() const   {   return textureID;   }
        inline std::string GetTexturePath() const { return texture_path; }
        inline std::string GetTextureName() const { return textureName; }
        inline int GetTextureWidth() const { return width; }
        inline int GetTextureHeight() const { return height; }
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
};

#endif // !TEXTURE_H