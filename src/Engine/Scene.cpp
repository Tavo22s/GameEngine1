#include "Scene.hpp"
#include "Components/MeshRender.hpp"
#include "Components/SkinnedMeshRender.hpp"
#include "Components/Animator.hpp"
#include "Utils.hpp"
#include "Time.hpp"
#include "Input.hpp"

unsigned int VAO;
std::vector<Animation*> vanim;
GameObject *cameraParent;
GameObject *cameraOBJ;
Animator *animator;
GameObject *light;

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
    cameraParent = new GameObject("parentcamera", this);
    cameraOBJ = new GameObject("camera", this);
    main_camera = cameraOBJ->AddComponent<Camera>();
    cameraParent->transform->Translate(.0f, 1.f, -5.f);

    cameraParent->AddChildren(cameraOBJ);
    gameObjects.push_back(cameraParent);

    Mesh *mesh = LoadMesh(Utils::attach_strings(Utils::Path, "assets\\models\\Mutant\\Mutant.fbx").c_str());
    Shader *shader = LoadShader(Utils::attach_strings(Utils::Path, "assets/shaders/SimpleVertexShader.glsl").c_str(),
        Utils::attach_strings(Utils::Path, "assets/shaders/SimpleFragmentShader.glsl").c_str());
    vanim = LoadAnimation(Utils::attach_strings(Utils::Path, "assets\\models\\Mutant\\Mutant.fbx").c_str());
    vanim[3]->speed = .5f;

    light = new GameObject("light1", this);
    light->transform->Translate(0.f, 1.f, -1.f);
    Light *l1 = light->AddComponent<Light> ();
    l1->color = glm::vec3(10.f, 0.0f, 0.0f);
    l1->type_light = SPOT;
    gameObjects.push_back(light);

    GameObject *light2 = new GameObject("light2", this);
    Light *l2 = light2->AddComponent<Light>();
    l2->color = glm::vec3(10, 10, 10);
    gameObjects.push_back(light2);

    GameObject *parent = new GameObject("cjParent", this);
    GameObject *cj = new GameObject("cj", this, false);
    SkinnedMeshRender *meshrender = cj->AddComponent<SkinnedMeshRender> ();
    animator = cj->AddComponent<Animator> ();
    animator->SetSkinnedMeshRender(meshrender);
    animator->SetMesh(mesh);
    animator->SetAnimation(vanim[2]);

    meshrender->SetMesh(mesh);
    meshrender->SetShader(shader);

    parent->AddChildren(cj);

    cj->transform->Rotate(0.f, 180.0f, .0f);
    cj->transform->Scale(.01f, .01f, .01f);

    parent->childrens.push_back(cj);
    gameObjects.push_back(parent);
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
    if(Input.GetKeyDown('W'))
        animator->SetAnimation(vanim[3]);
    if(Input.GetKeyUp('W'))
        animator->SetAnimation(vanim[2]);

    //light->transform->Rotate(.0f, 100 * Time.deltaTime, 0.f);
}