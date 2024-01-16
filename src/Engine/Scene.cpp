#include "Scene.hpp"

Scene::Scene(std::string _name)
{
    name = _name;
}

Scene::~Scene()
{
    for(auto g:gameObjects)
        delete g;
}

void Scene::Init()
{
    GameObject *cameraOBJ = new GameObject("camera", this);
    main_camera = cameraOBJ->AddComponent<Camera>();
    gameObjects.push_back(cameraOBJ);
    cameraOBJ->transform->Translate(.0f, .0f, -10.f);

    for(auto g:gameObjects)
        g->Init();
}

void Scene::Render()
{
    for(auto g:gameObjects)
        g->Render();
}

void Scene::Update()
{
    for(auto g:gameObjects)
        g->Update();
}