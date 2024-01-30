#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "Component.hpp"
#include "../Render/Texture.hpp"

class Camera:public Component
{
    public:
        Camera(GameObject *gO, Scene* sn);
        ~Camera();

        void Init();
        void Update();
        void BeginFrameBuffer();
        void EndFrameBuffer();
        void RenderScreen();

        float FOV;
        float near;
        float far;
        unsigned int width;
        unsigned int height;
        glm::mat4 viewMat;
        glm::mat4 projMat;
        Texture *screen_tex;
    private:
        float aspect;
        unsigned int m_UIFramebuffer;
        unsigned int m_UIRenderbuffer;
};

#endif //CAMERA_H