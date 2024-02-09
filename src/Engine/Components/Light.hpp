#ifndef LIGHT_H
#define LIGHT_H

#include "Component.hpp"
#include <glm/glm.hpp>
#include "../Render/Shader.hpp"

enum TypeLight
{
    DIRECCTIONAL = 0,
    POINT = 1,
    SPOT = 2
};

class Light: public Component
{
    public:
        Light(GameObject *gO, Scene *sn);
        ~Light();
        
        void Init();

        void SetPhong(float _constant, float _linear, float _quadratic);
        void SetAngle(float _cutOff, float _outerCutOff);

        void UseLight(Shader *, unsigned int);

        TypeLight type_light;
        glm::vec3 color;
    private:
        float constant;
        float linear;
        float quadratic;

        float cutOff;
        float outerCutOff;
};

#endif // !LIGHT_H