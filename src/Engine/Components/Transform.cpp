#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Gameobject.hpp"

Transform::Transform(GameObject *gO, Scene* sn):Component("transform", gO, sn)
{
    position = glm::vec3(.0f, .0f, .0f);
    scale = glm::vec3(1.f, 1.f, 1.f);
    rotation = glm::quat(glm::radians(glm::vec3(.0f, .0, .0f)));
}

Transform::~Transform()
{
}

void Transform::Init()
{
    glm::mat4 posMat = glm::translate(glm::mat4(1.f), position);
    glm::mat4 rotMat = glm::mat4_cast(rotation);
    glm::mat4 sclMat = glm::scale(glm::mat4(1.f), scale);

    model = posMat * rotMat * sclMat;
}

void Transform::Update()
{
    model = glm::translate(glm::mat4(1.f), position) *
        glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.f), scale);
}

void Transform::Translate(glm::vec3 pos)
{
    position += pos;
}

void Transform::Translate(float x, float y, float z)
{
    position += glm::vec3(x, y, z);
}

void Transform::Rotate(glm::vec3 rot)
{
    rotation *= glm::quat(glm::radians(rot));
}

void Transform::Rotate(float x, float y, float z)
{
    rotation *= glm::quat(glm::radians(glm::vec3(x, y, z)));
}

void Transform::Rotate(glm::quat quat)
{
    rotation *= quat;
}

void Transform::Rotate(float x, float y, float z, float w)
{
    rotation *= glm::quat(w, x, y, z);
}

void Transform::Scale(glm::vec3 scl)
{
    scale = scl;
}

void Transform::Scale(float x, float y, float z)
{
    scale = glm::vec3(x, y, z);
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