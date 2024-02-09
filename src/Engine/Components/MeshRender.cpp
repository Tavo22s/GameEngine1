#include "MeshRender.hpp"
#include "../Scene.hpp"
#include "../Utils.hpp"

MeshRender::MeshRender(GameObject *gO, Scene *sn):Component("meshrender", gO, sn)
{

}

MeshRender::~MeshRender()
{
}

void MeshRender::Render()
{
    if(m_pShader && m_pMesh)
    {
        m_pShader->use();
        m_pShader->setMat4("projection", scene->main_camera->projMat);
        m_pShader->setMat4("view", scene->main_camera->viewMat);
        m_pShader->setMat4("model", gameObject->transform->GetOpenGLMat());
        m_pShader->setVec3("camPos", scene->main_camera->gameObject->transform->GetPosition());
        m_pShader->setInt("num_lights", scene->lights.size());

        for(std::size_t i=0; i < scene->lights.size(); i++)
            scene->lights[i]->UseLight(m_pShader, i);

        m_pMesh->Render(m_pShader);
    }
}