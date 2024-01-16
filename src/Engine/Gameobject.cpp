#include "Gameobject.hpp"
#include "Components/Camera.hpp"

GameObject::GameObject(std::string _name, Scene *sn, bool _isStatic)
{
    name = _name;
    scene = sn;
    isStatic = _isStatic;
    transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
    for(auto c:components)
    {
        if(c)
            delete c;
    }
}

void GameObject::Init()
{
    for(auto c:components)
    {
        c->Init();
    }
}

void GameObject::Render()
{
    for(auto c:components)
        c->Render();
}

void GameObject::Update()
{
    for(auto c:components)
        c->Update();
}