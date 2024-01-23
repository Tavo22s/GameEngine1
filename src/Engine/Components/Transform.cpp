#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Gameobject.hpp"

Transform::Transform(GameObject *gO, Scene* sn):Component("transform", gO, sn)
{
    position = localPosition = glm::vec3(.0f, .0f, .0f);
    scale = localScale = glm::vec3(1.f, 1.f, 1.f);
    rotation = localRotation = glm::quat(glm::radians(glm::vec3(.0f, .0, .0f)));
}

Transform::~Transform()
{
}

void Transform::Init()
{
    glm::mat4 posMat = glm::translate(glm::mat4(1.f), localPosition);
    glm::mat4 rotMat = glm::mat4_cast(localRotation);
    glm::mat4 sclMat = glm::scale(glm::mat4(1.f), localScale);

    model = posMat * rotMat * sclMat;
}

void Transform::Update()
{
    glm::mat4 tmp = GetLocalModelMatrix();
    if(gameObject->parent)
        model = gameObject->parent->transform->GetLocalModelMatrix() * tmp;
    else
        model = tmp;
    position = model[3];
}

void Transform::Translate(glm::vec3 pos)
{
    localPosition += pos;
}

void Transform::Translate(float x, float y, float z)
{
    localPosition += glm::vec3(x, y, z);
}

void Transform::Rotate(glm::vec3 rot)
{
    localRotation *= glm::quat(glm::radians(rot));
}

void Transform::Rotate(float x, float y, float z)
{
    localRotation *= glm::quat(glm::radians(glm::vec3(x, y, z)));
}

void Transform::Rotate(glm::quat quat)
{
    localRotation *= quat;
}

void Transform::Rotate(float x, float y, float z, float w)
{
    localRotation *= glm::quat(w, x, y, z);
}

void Transform::Scale(glm::vec3 scl)
{
    localScale = scl;
}

void Transform::Scale(float x, float y, float z)
{
    localScale = glm::vec3(x, y, z);
}

glm::vec3 Transform::Front() const
{
    return glm::normalize(glm::vec3(model * glm::vec4(.0f, .0f, 1.f, .0f)));
}

glm::vec3 Transform::Up() const
{
    return glm::normalize(glm::vec3(model * glm::vec4(.0f, 1.f, .0f, .0f)));
}

glm::vec3 Transform::Right() const
{
    return glm::normalize(glm::vec3(model * glm::vec4(1.f, .0f, .0f, .0f)));
}

glm::mat4 Transform::GetLocalModelMatrix()
{
    return glm::translate(glm::mat4(1.f), localPosition) *
        glm::mat4_cast(localRotation) * glm::scale(glm::mat4(1.f), localScale);
}