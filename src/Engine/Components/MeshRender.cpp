#include "MeshRender.hpp"

MeshRender::MeshRender(GameObject *gO, Scene *sn):Component(gO, sn)
{

}

MeshRender::~MeshRenderÑ()
{
}

void MeshRender::Render()
{
    m_pShader->use();
}