#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "Texture.hpp"
#include "Shader.hpp"

class Material
{
    public:
        Material();
        ~Material();

        void Render(Shader *);

        friend Material *LoadMaterial();

        glm::vec3 albedo;
        float metallic;
        float roughness;
        float ao;

        Texture *albedoMap;
        Texture *normalMap;
        Texture *metallicMap;
        Texture *roughnessMap;
        Texture *aoMap;        
};
#endif // !MATERIAL_H