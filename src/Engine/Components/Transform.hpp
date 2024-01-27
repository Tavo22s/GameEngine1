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

        void SetTransform(glm::mat4);

        inline glm::mat4 GetOpenGLMat()    const   {   return model;   }

        inline glm::vec3 GetLocalPosition() const   {   return localPosition;   }
        inline glm::quat GetLocalRotation() const   {   return localRotation;   }
        inline glm::vec3 GetLocalScale()    const   {   return localScale;  }

        inline glm::vec3 GetPosition() const   {   return position;    }
        inline glm::quat GetRotation() const   {   return rotation;    }
        inline glm::vec3 GetScale() const   {   return scale;   }

        glm::vec3 Front() const;
        glm::vec3 Up() const;
        glm::vec3 Right() const;      

    private:
        glm::mat4 model;

        //Local
        glm::vec3 localPosition;
        glm::quat localRotation;
        glm::vec3 localScale;

        //Wordl
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;

        glm::mat4 GetLocalModelMatrix();
};

#endif //TRANSFORM_H