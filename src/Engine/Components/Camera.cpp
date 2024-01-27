#include "Camera.hpp"
#include "../Configs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Gameobject.hpp"

Camera::Camera(GameObject *gO, Scene* sn):Component("camera", gO, sn)
{
    aspect = float(Config::WIDTH) / float(Config::HEIGHT);
    FOV = 45.f;
    near = 0.1f;
    far = 100.f;
}

Camera::~Camera()
{
}

void Camera::Init()
{
    projMat = glm::perspective(glm::radians(FOV), aspect, near, far);
    viewMat = glm::lookAt(gameObject->transform->GetPosition(), 
        gameObject->transform->GetPosition() + gameObject->transform->Front(),
        gameObject->transform->Up());
}

void Camera::Update()
{
    projMat = glm::perspective(glm::radians(FOV), aspect, near, far);
    viewMat = glm::lookAt(gameObject->transform->GetPosition(), 
        gameObject->transform->GetPosition() + gameObject->transform->Front(),
        gameObject->transform->Up());
}