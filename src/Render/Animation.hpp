#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <vector>

struct TransformKey
{
    double m_dTime;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

class Animation
{
    public:
        ~Animation();

        friend std::vector<Animation *>LoadAnimation(std::string);

        std::string name;
        std::vector<glm::mat4> transforms;

        inline double GetDuration() const   {   return duration;    }
        inline double GetSpeed()    const   {   return speed;   }
        inline double GetTicksPerSecond()   const   {   return ticks_per_second;    }
        inline std::map<std::string, std::vector<TransformKey*>> GetRootKeys()  const   {   return RootKeys;    }

    private:
        Animation();
        std::map<std::string, std::vector<TransformKey*>> RootKeys;
        double duration;
        double speed;
        double ticks_per_second;
};

std::vector<Animation *>LoadAnimation(std::string);

#endif //ANIMATION_H