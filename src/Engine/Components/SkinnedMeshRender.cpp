#include "SkinnedMeshRender.hpp"
#include "../Scene.hpp"
#include "../Gameobject.hpp"
#include "../Utils.hpp"

SkinnedMeshRender::SkinnedMeshRender(GameObject *gO, Scene *sn):Component("skinnedmeshrender", gO, sn)
{
}

SkinnedMeshRender::~SkinnedMeshRender()
{
}

void SkinnedMeshRender::Init()
{
    gameObject->childrens.push_back(m_pMesh->GetMakedModel(scene));
}

void SkinnedMeshRender::Render()
{
    if(m_pShader && m_pMesh)
    {
        m_pShader->use();
        for(unsigned int i=0; i<transforms.size(); i++)
        {
            std::string name1 = Utils::attach_strings("bones[", Utils::to_string(i));
            std::string namef = Utils::attach_strings(name1, "]");
            m_pShader->setMat4(namef, transforms[i]);
        }
        m_pShader->setMat4("projection", scene->main_camera->projMat);
        m_pShader->setMat4("view", scene->main_camera->viewMat);
        m_pShader->setMat4("model", gameObject->transform->GetOpenGLMat());
        m_pMesh->Render(m_pShader);
    }
}