#include "Mesh.hpp"
#include "../Engine/Utils.hpp"

Mesh::Mesh()
{
    m_BoneCounter = 0;
    m_num_bones = 0;
}

void Mesh::Render(Shader *shader)
{
    for(auto s:submeshes)
        s->Render(shader);
}

bool Mesh::loadModel(std::string _path, bool gamma)
{
    gammaCorrection = gamma;
    Assimp::Importer importer;
    _path = std::string(_path);
    #ifdef __linux__
        Utils::FixPathToLinux(_path);
    #endif
    std::cout<<_path<<std::endl;
    complet_path = _path;
    const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout<<"ERROR::ASSIMP:: "<<importer.GetErrorString()<<std::endl;
        return false;
    }

    m_global_inverse_transform = aiMatrix4x4ToGlm(&scene->mRootNode->mTransformation);
    m_global_inverse_transform = glm::inverse(m_global_inverse_transform);

    #ifdef __linux__
        path = _path.substr(0, _path.find_last_of('/'));
    #else
        path = _path.substr(0, _path.find_last_of('\\'));
    #endif
    processNode(scene->mRootNode, scene);
    return true;
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i=0; i<node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        submeshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

SubMesh* Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    for(unsigned int i=0; i<mesh->mNumVertices; i++)
    {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if(mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(.0f, .0f);
        vertices.push_back(vertex);
    }

    for(unsigned int i=0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture*> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture*> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture*> normalMaps= loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture*> heightMaps = loadMaterialTexture(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    ExtractBoneWeightForVertices(vertices, mesh, scene);

    for(unsigned int i=0;i<mesh->mNumBones; i++)
    {
        unsigned int bone_index = 0;
        std::string bone_name(mesh->mBones[i]->mName.data);
        if(m_bone_mapping.find(bone_name) == m_bone_mapping.end())
        {
            bone_index = m_num_bones;
            m_num_bones++;
            BoneMatrix bi;
            m_bone_matrices.push_back(bi);
            m_bone_matrices[bone_index].offset_matrix = aiMatrix4x4ToGlm(&mesh->mBones[i]->mOffsetMatrix);
            m_bone_mapping[bone_name] = bone_index;
        }
        else
            bone_index = m_bone_mapping[bone_name];
    }

    return new SubMesh(vertices, indices, textures);
}

std::vector<Texture*> Mesh::loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture*> textures;

    for(unsigned int i=0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j]->GetTexturePath().data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture *texture;
            std::string filename = std::string(str.C_Str());
            std::string tmp = Utils::attach_strings(path, "/");
            filename = Utils::attach_strings(tmp, filename);
            texture = LoadTexture(filename);
            texture->type = typeName;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Mesh *LoadMesh(const std::string &path, bool gamma)
{
    Mesh *mesh = new Mesh();
    if(mesh)
    {
        if(mesh->loadModel(path, gamma))
        {
            return mesh;
        }
    }
    return nullptr;
}

GameObject *Mesh::GetMakedModel(Scene *sn)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(complet_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    return ProcessNode(scene->mRootNode, sn);
}

GameObject *Mesh::ProcessNode(aiNode *pNode, Scene *sn, GameObject *parent)
{
    aiMatrix4x4 mat = pNode->mTransformation;
    aiVector3D pos, scl;
    aiQuaternion quat;

    mat.Decompose(scl, quat, pos);
    glm::mat4 transform = aiMatrix4x4ToGlm(&mat);
    GameObject *go = new GameObject(pNode->mName.C_Str(), sn);
    go->Init();
    go->parent = parent;
    go->transform->SetTransform(transform);
    unsigned int n = pNode->mNumChildren;
    for(unsigned int i=0; i<n; i++)
        go->AddChildren(ProcessNode(pNode->mChildren[i], sn, go));
    return go;
}

void Mesh::SetVertexBoneDataToDefault(Vertex &vertex)
{
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_fWeights[i] = .0f;
    }
}

void Mesh::SetVertexBoneData(Vertex &vertex, int boneID, float weight)
{
    for(int i=0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if(vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_fWeights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void Mesh::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh *mesh, const aiScene *scene)
{
    auto& boneInfoMap = m_boneInfoMap;
    int& boneCount = m_BoneCounter;

    for(int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if(boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCount;
            newBoneInfo.offset = aiMatrix4x4ToGlm(&mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
            boneID = boneInfoMap[boneName].id;
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for(int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexID = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexID <= vertices.size());
            SetVertexBoneData(vertices[vertexID], boneID, weight);
        }
    }
}