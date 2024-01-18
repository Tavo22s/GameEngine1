#ifndef MESHRENDER_H
#define MESHRENDER_H

#include "Component.hpp"
#include "../Render/Mesh.cpp"

class MeshRender:public Component
{
    public:
        MeshRender(GameObject *gO, Scene *sn);
        ~MeshRender();

        void Render();

        inline void SetShader(Shader *shader)   const   {  m_pShader = shader;  }
        inline void SetMesh(Mesh *mesh)  const  {   m_pMesh = Mesh; }
    private:
        Shader *m_pShader;
        Mesh *m_pMesh;
};

#endif //MESHRENDER_H