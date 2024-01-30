#include <glad/glad.h>
#include "Camera.hpp"
#include "../Configs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Gameobject.hpp"

Camera::Camera(GameObject *gO, Scene* sn):Component("camera", gO, sn)
{
    width = Config::WIDTH;
    height = Config::HEIGHT;
    aspect = float(width) / float(height);
    FOV = 45.f;
    near = 0.1f;
    far = 100.f;
}

Camera::~Camera()
{
}

void Camera::Init()
{
    aspect = float(width) / float(height);
    projMat = glm::perspective(glm::radians(FOV), aspect, near, far);
    viewMat = glm::lookAt(gameObject->transform->GetPosition(), 
        gameObject->transform->GetPosition() + gameObject->transform->Front(),
        gameObject->transform->Up());

    glGenFramebuffers(1, &m_UIFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_UIFramebuffer);
    screen_tex = LoadVoidTexture(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_tex->GetID(), 0);

    glGenRenderbuffers(1, &m_UIRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_UIRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_UIRenderbuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout<<"ERROR::FRAMEBUFFER:: Framebuffer is not complete!"<<std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::Update()
{
    aspect = float(width) / float(height);
    projMat = glm::perspective(glm::radians(FOV), aspect, near, far);
    viewMat = glm::lookAt(gameObject->transform->GetPosition(), 
        gameObject->transform->GetPosition() + gameObject->transform->Front(),
        gameObject->transform->Up());
}

void Camera::BeginFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_UIFramebuffer);
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::EndFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::RenderScreen()
{
}