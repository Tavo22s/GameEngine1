#ifndef SYSTEM_H
#define SYSTEM_H

#include <GLFW/glfw3.h>
#include "Scene.hpp"
#include "ResourcesManager.hpp"

class System
{
    public:
        System(unsigned int, unsigned int);
        ~System();

        bool Init();
        void Loop();
        void Shutdown();

        void ViewportResize(int, int);
        void KeyController(int, int);
        void MousePosition(double, double);
        void MouseButton(int, int);
        void Scroll(double);

    private:
        bool Render();
        bool Update();
        void InitGui();
        void RenderGUI();

        GLFWwindow *m_pWindow;
        Scene *m_pScene;
        ResourcesManager *m_pResourcesManager;

        unsigned int m_iWidth;
        unsigned int m_iHeight;

        inline static auto WindowResizeCallback(GLFWwindow *win, int w, int h)->void
        {
            System *sys = static_cast<System*> (glfwGetWindowUserPointer(win));
            sys->ViewportResize(w, h);
        }

        inline static auto WindowKeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods)->void
        {
            System *sys = static_cast<System*> (glfwGetWindowUserPointer(win));
            sys->KeyController(key, action);
        }

        inline static auto WindowMousePositionCallback(GLFWwindow *win, double xPos, double yPos)->void
        {
            System *sys = static_cast<System*> (glfwGetWindowUserPointer(win));
            sys->MousePosition(xPos, yPos);
        }

        inline static auto WindowMouseButtonCallback(GLFWwindow *win, int button, int action, int mods)->void
        {
            System *sys = static_cast<System*> (glfwGetWindowUserPointer(win));
            sys->MouseButton(button, action);
        }

        inline static auto WindowScrollCallback(GLFWwindow *win, double xoffset, double yoffset)
        {
            System *sys = static_cast<System*> (glfwGetWindowUserPointer(win));
            sys->Scroll(yoffset);
        }
};

#endif //SYSTEM_H