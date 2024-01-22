#include "Scene.hpp"
#include "Components/MeshRender.hpp"
#include "Utils.hpp"
#include "Time.hpp"
#include "Input.hpp"

unsigned int VAO;

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
    cameraOBJ->transform->Translate(.0f, .0f, -2.f);
    
    Mesh *mesh = new Mesh(Utils::attach_strings(Utils::Path, "assets/models/cj/CJ_GTASA.fbx").c_str());
    Shader *shader = LoadShader(Utils::attach_strings(Utils::Path, "assets/shaders/SimpleVertexShader.glsl").c_str(),
        Utils::attach_strings(Utils::Path, "assets/shaders/SimpleFragmentShader.glsl").c_str());

    GameObject *cj = new GameObject("cj", this);
    MeshRender *meshrender = cj->AddComponent<MeshRender> ();
    meshrender->SetMesh(mesh);
    meshrender->SetShader(shader);

    cj->transform->Rotate(90.f, 180.0f, .0f);

    gameObjects.push_back(cj);

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