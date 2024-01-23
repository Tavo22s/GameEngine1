#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include "System.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "stb_image.h"
#include "Configs.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>

System::System(unsigned int width, unsigned int height)
{
    m_iWidth = width;
    m_iHeight = height;
    m_pWindow = nullptr;
    m_pResourcesManager = nullptr;
    Config::WIDTH = width;
    Config::HEIGHT = height;
}

System::~System()
{
}

bool System::Init()
{
    srand(time(0));
    if(!glfwInit())
    {
        std::cout<<"Error to initialize GLFW"<<std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, "Engine", nullptr, nullptr);
    if(!m_pWindow)
    {
        std::cout<<"Error to create Window"<<std::endl;
        return false;
    }

    glfwMakeContextCurrent(m_pWindow);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetFramebufferSizeCallback(m_pWindow, WindowResizeCallback);
    glfwSetKeyCallback(m_pWindow, WindowKeyCallback);
    glfwSetCursorPosCallback(m_pWindow, WindowMousePositionCallback);
    glfwSetMouseButtonCallback(m_pWindow, WindowMouseButtonCallback);
    glfwSetScrollCallback(m_pWindow, WindowScrollCallback);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"Error to intialize OpenGL"<<std::endl;
        return false;
    }

    InitGui();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Time.time = glfwGetTime();
    Time.lastFrame = Time.time;

    Input = __INPUT__();

    m_pScene = new Scene("test");
    m_pScene->Init();

    return true;
}

void System::Loop()
{
    while(!glfwWindowShouldClose(m_pWindow))
    {
        glClearColor(.0f, .0f, .0f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Update();
        Render();
        RenderGUI();

        Input.Update();

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}

void System::Shutdown()
{
    glfwDestroyWindow(m_pWindow);
}

void System::ViewportResize(int w, int h)
{
    glViewport(0, 0, w, h);
    m_iWidth = w;
    m_iHeight = h;
    Config::WIDTH = w;
    Config::HEIGHT = h;
}

void System::KeyController(int key, int action)
{
    switch (action)
    {
        case 0:
            Input.SetKeyUp(key);
            break;
        case 1:
            Input.SetKeyDown(key);
            break;
        case 2:
            Input.keys[key] = KEY_PRESS;
            break;
        default:
            break;
    }
}

void System::MousePosition(double xPos, double yPos)
{
    Input.mousePosition.x = float(xPos);
    Input.mousePosition.y = float(yPos);
}

void System::MouseButton(int button, int action)
{
    if(action)
        Input.buttons[button] = KEY_DOWN;
    else
        Input.buttons[button] = KEY_UP;

}

void System::Scroll(double yoffset)
{
}

bool System::Render()
{
    m_pScene->Render();
    return true;
}

bool System::Update()
{
    Time.time = glfwGetTime();
    Time.deltaTime = Time.time - Time.lastFrame;
    Time.lastFrame = Time.time;

    m_pScene->Update();

    return true;
}


void System::InitGui()
{
    const char *glsl_version = "#version 330 core";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &_io = ImGui::GetIO(); (void) _io;
    _io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void System::RenderGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //RenderGUI

    ImGui::Begin("Test");
    ImGui::Text("Time: %.2f", Time.time);
    ImGui::Text("Delta Time: %.2f", Time.deltaTime);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}