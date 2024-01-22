#ifndef MESHRENDER_H
#define MESHRENDER_H

#include "Component.hpp"
#include "../Render/Mesh.hpp"

class MeshRender:public Component
{
    public:
        MeshRender(GameObject *gO, Scene *sn);
        ~MeshRender();

        void Render();

        inline void SetShader(Shader *shader)   {  m_pShader = shader;  }
        inline void SetMesh(Mesh *mesh) {   m_pMesh = mesh; }
    private:
        Shader *m_pShader;
        Mesh *m_pMesh;
};

#endif //MESHRENDER_H