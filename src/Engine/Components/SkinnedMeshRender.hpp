#ifndef SKINNEDMESHRENDER_H
#define SKINNEDMESHRENDER_H

#include "Component.hpp"
#include "../Render/Mesh.hpp"

class SkinnedMeshRender: public Component
{
    public:
        SkinnedMeshRender(GameObject *gO, Scene* sn);
        ~SkinnedMeshRender();

        void Init();
        void Render();

        inline void SetShader(Shader *shader)   {  m_pShader = shader;  }
        inline void SetMesh(Mesh *mesh) {   m_pMesh = mesh; }

        inline Mesh *GetMesh()  const   {   return m_pMesh; }
        std::vector<glm::mat4> transforms;
    private:
        Shader *m_pShader;
        Mesh *m_pMesh;
        
};

#endif //SKINNEDMESHRENDER_H