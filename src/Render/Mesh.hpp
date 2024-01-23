#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SubMesh.hpp"

class Mesh
{
    public:
        ~Mesh();

        void Render(Shader *shader);

        std::vector<Texture*> textures_loaded;
        std::vector<SubMesh*> submeshes;
        std::string path;
        bool gammaCorrection;
        std::string name;

        friend Mesh *LoadMesh(const std::string &path, bool gamma);
    private:
        Mesh();
        bool loadModel(std::string path, bool gamma);
        void processNode(aiNode *node, const aiScene *scene);
        SubMesh *processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture*> loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName);
};

Mesh *LoadMesh(const std::string &path, bool gamma = false);

#endif //MESH_H