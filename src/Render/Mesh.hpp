#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SubMesh.hpp"

#include "../Engine/Gameobject.hpp"
#include <map>

struct BoneMatrix
{
    glm::mat4 offset_matrix;
    glm::mat4 final_world_transform;
};

struct BoneInfo
{
    unsigned int id;
    glm::mat4 offset;
};

class Mesh
{
    public:
        ~Mesh();

        void Render(Shader *shader);
        friend Mesh *LoadMesh(const std::string &path, bool gamma);
        GameObject *GetMakedModel(Scene *);

        std::vector<Texture*> textures_loaded;
        std::vector<SubMesh*> submeshes;
        std::string path;
        bool gammaCorrection;
        std::string name;

        unsigned int m_num_bones;
        std::map<std::string, unsigned int> m_bone_mapping;
        std::vector<BoneMatrix> m_bone_matrices;
        glm::mat4 m_global_inverse_transform;
    private:
        Mesh();
        bool loadModel(std::string path, bool gamma);
        void processNode(aiNode *node, const aiScene *scene);
        SubMesh *processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture*> loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName);
        GameObject *ProcessNode(aiNode *pNode, Scene *scene, GameObject *parent = nullptr);
        void SetVertexBoneDataToDefault(Vertex &vertex);
        void SetVertexBoneData(Vertex &vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh *mesh, const aiScene *scene);

        const aiScene *scene;
        std::map<std::string, BoneInfo> m_boneInfoMap;
        int m_BoneCounter;
};

Mesh *LoadMesh(const std::string &path, bool gamma = false);

#endif //MESH_H