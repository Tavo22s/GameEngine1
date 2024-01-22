#include <glad/glad.h>
#include "SubMesh.hpp"
#include "../Engine/Utils.hpp"

SubMesh::SubMesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture*> _textures)
{
    this->vertices = _vertices;
    this->indices = _indices;
    this->textures = _textures;
    m_UIVAO = m_UIVBO = m_UIEBO = 0;
    SetupMesh();
}

void SubMesh::Render(Shader *shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i]->type;
        if(name == "texture_diffuse")
            number = Utils::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = Utils::to_string(specularNr++);
        else if(name == "texture_normal")
            number = Utils::to_string(normalNr++);
        else if(name == "texture_height")
            number = Utils::to_string(heightNr++);
        std::string str = Utils::attach_strings(name, number);
        shader->setInt(Utils::attach_strings(str, number), i);
        textures[i]->Render();
    }

    glBindVertexArray(m_UIVAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void SubMesh::SetupMesh()
{
    glGenVertexArrays(1, &m_UIVAO);
    glGenBuffers(1, &m_UIVBO);
    glGenBuffers(1, &m_UIEBO);

    glBindVertexArray(m_UIVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_UIVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_UIEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Bitangent));

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*) offsetof(Vertex, m_BoneIDs));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_fWeights));

    glBindVertexArray(0);
}