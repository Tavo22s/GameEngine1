#ifndef SCENE_H
#define SCENE_H

#include "Components/Camera.hpp"
#include "Gameobject.hpp"

class Scene
{
    public:
        Scene(std::string _name);
        ~Scene();

        void Init();
        void Render();
        void Update();
    
        std::vector<GameObject*> gameObjects;
        std::string name;

        Camera *main_camera;
};

#endif //SCENE_H