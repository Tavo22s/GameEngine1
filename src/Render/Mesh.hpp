#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SubMesh.hpp"

class Mesh
{
    public:
        Mesh(std::string const &path, bool gamma = false);
        ~Mesh();

        void Render(Shader *shader);

        std::vector<Texture*> textures_loaded;
        std::vector<SubMesh*> submeshes;
        std::string path;
        bool gammaCorrection;
    private:
        void loadModel(std::string const &path);
        void processNode(aiNode *node, const aiScene *scene);
        SubMesh *processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture*> loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName);

};

#endif //MESH_H