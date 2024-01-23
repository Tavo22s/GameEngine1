#include "Mesh.hpp"
#include "../Engine/Utils.hpp"

Mesh::Mesh()
{

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
    const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout<<"ERROR::ASSIMP:: "<<importer.GetErrorString()<<std::endl;
        return false;
    }
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