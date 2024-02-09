#include "Light.hpp"
#include "../Gameobject.hpp"
#include "../Scene.hpp"
#include "../Utils.hpp"

Light::Light(GameObject *gO, Scene *sn):Component("Light", gO, sn)
{
    type_light = DIRECCTIONAL;
    this->constant = 1.0;
    this->linear = 0.14;
    this->quadratic = 0.07;
    this->color = glm::vec3(1.f);
    cutOff = 12.5f;
    outerCutOff = 15.f;
}

Light::~Light()
{
}

void Light::Init()
{
    scene->lights.push_back(this);
}

void Light::SetPhong(float _constant, float _linear, float _quadratic)
{
    constant = _constant;
    linear = _linear;
    quadratic = _quadratic;
}

void Light::SetAngle(float _cutOff, float _outerCutOff)
{
    cutOff = _cutOff;
    outerCutOff = _outerCutOff;
}

void Light::UseLight(Shader *pShader, unsigned int idx)
{
    std::string idxs = Utils::to_string(idx);
    
    pShader->setVec3("lights[" + idxs + "].position", gameObject->transform->GetPosition());
    pShader->setVec3("lights[" + idxs + "].direction", gameObject->transform->Front());
    pShader->setVec3("lights[" + idxs + "].color", color);
    pShader->setFloat("lights[" + idxs+ "].constant", constant);
    pShader->setFloat("lights[" + idxs+ "].linear", linear);
    pShader->setFloat("lights[" + idxs+ "].quadratic", quadratic);
    pShader->setFloat("lights[" + idxs+ "].cutOff", glm::cos(glm::radians(cutOff)));
    pShader->setFloat("lights[" + idxs+ "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
    pShader->setInt("lights[" + idxs + "].type", type_light);
}