#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Components/Component.hpp"
#include "Components/Transform.hpp"
#include <vector>

class Scene;

class GameObject
{
    public:
        GameObject(std::string _name, Scene *sn, bool _isStatic = false);
        ~GameObject();

        void Init();
        void Render();
        void Update();

        template<class T>
        inline T *AddComponent()
        {
            T *component = new T(this, scene);
            components.push_back(component);
            return component;
        }

        template<class T>
        inline T *AddComponent(T* component)
        {
            components.push_back(component);
            return component;
        }

        template<class T>
        inline T *GetComponent()
        {
            T var = T(nullptr, nullptr);
            for(auto c:components)
            {
                if(var.name == c->name)
                    return c;
            }
            return nullptr;
        }

        inline GameObject *AddChildren(GameObject *child)
        {
            if(child)
            {
                childrens.push_back(child);
                child->parent = this;
                return child;
            }
            return nullptr;
        }

        std::vector<Component*> components;
        std::vector<GameObject*> childrens;

        GameObject *parent;

        Transform *transform;
        Scene *scene;
        std::string name;
        bool isStatic;
};

#endif //GAMEOBJECT_H