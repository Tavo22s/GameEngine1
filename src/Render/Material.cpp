#include <glad/glad.h>
#include "Material.hpp"

Material::Material()
{
    albedo = glm::vec3(1.f);
    metallic = 0.0f;
    roughness = 0.0f;
    ao = 0.1f;

    albedoMap = nullptr;
    normalMap = nullptr;
    metallicMap = nullptr;
    roughnessMap = nullptr;
    aoMap = nullptr;
}

Material::~Material()
{
}

void Material::Render(Shader *pShader)
{
    if(albedoMap)
    {
        glActiveTexture(GL_TEXTURE0);
        pShader->setInt("material.albedoMap", 0);
        pShader->setBool("material.aldedoMapEnable", true);
        albedoMap->Render();
    }
    else
        pShader->setVec3("material.albedo", albedo);
    if(normalMap)
    {
        glActiveTexture(GL_TEXTURE1);
        pShader->setInt("material.normalMap", 1);
        pShader->setBool("materila.normalMapEnable", true);
        normalMap->Render();
    }
    if(metallicMap)
    {
        glActiveTexture(GL_TEXTURE2);
        pShader->setInt("material.metallicMap", 2);
        pShader->setBool("material.metallicMapEnable", true);
        albedoMap->Render();
    }
    else
        pShader->setFloat("material.metallic", metallic);
    if(roughnessMap)
    {
        glActiveTexture(GL_TEXTURE3);
        pShader->setInt("material.roughnessMap", 3);
        pShader->setBool("material.roughnessMapEnable", true);
        roughnessMap->Render();
    }
    else
        pShader->setFloat("material.roughness", roughness);
    if(aoMap)
    {
        glActiveTexture(GL_TEXTURE4);
        pShader->setInt("material.aoMap", 4);
        pShader->setBool("material.aoMapEnable", true);
        aoMap->Render();
    }
    else
        pShader->setFloat("material.ao", ao);
}