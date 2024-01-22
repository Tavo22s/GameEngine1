#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.hpp"

class Transform: public Component
{
    public:
        Transform(GameObject *gO, Scene* sn);
        ~Transform();

        void Init();
        void Update();

        void Translate(glm::vec3);
        void Translate(float, float, float);

        void Rotate(glm::vec3);
        void Rotate(float, float, float);

        void Rotate(glm::quat);
        void Rotate(float, float, float, float);

        void Scale(glm::vec3);
        void Scale(float, float, float);

        inline glm::mat4 GetOpenGLMat()    const   {   return model;   }

        glm::vec3 Front() const;
        glm::vec3 Up() const;
        glm::vec3 Right() const;

        //Wordl
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;

    private:
        glm::mat4 model;

        glm::vec3 localPosition;
        glm::quat localRotation;
        glm::vec3 localScale;
};

#endif //TRANSFORM_H