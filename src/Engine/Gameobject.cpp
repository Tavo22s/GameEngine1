#include "Gameobject.hpp"
#include "Components/Camera.hpp"

GameObject::GameObject(std::string _name, Scene *sn, bool _isStatic)
{
    name = _name;
    scene = sn;
    isStatic = _isStatic;
    transform = AddComponent<Transform>();
    parent = nullptr;
}

GameObject::~GameObject()
{
    for(auto c:components)
    {
        if(c)
            delete c;
    }
    for(auto child: childrens)
        if(child)
            delete child;
}

void GameObject::Init()
{
    for(auto c:components)
    {
        c->Init();
    }
    for(auto child: childrens)
        child->Init();
}

void GameObject::Render()
{
    for(auto c:components)
        c->Render();
    for(auto child: childrens)
        child->Render();
}

void GameObject::Update()
{
    for(auto c:components)
        c->Update();
    for(auto child: childrens)
        child->Update();
        
}