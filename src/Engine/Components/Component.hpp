#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

class GameObject;
class Scene;

class Component
{
    public:
        inline Component(std::string _name, GameObject *gO, Scene *sn):name(_name)
        {
            gameObject = gO;
            scene = sn;
        }
        inline virtual ~Component() {}

        inline virtual void Init()  {}
        inline virtual void Render()    {}
        inline virtual void Update()    {}
    
        std::string name;
        Scene *scene;
        GameObject *gameObject;
};

#endif //COMPONENT_H