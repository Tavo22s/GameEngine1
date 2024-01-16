#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glm/glm.hpp>

class Shader
{
    public:
        Shader(const char*, const char*, const char* = nullptr);
        ~Shader();

        void use() const;
        void setBool(const std::string&, const bool&) const;
        void setInt(const std::string&, const int&) const;
        void setFloat(const std::string&, const float&) const;
        void setVec2(const std::string&, const glm::vec2 &) const;
        void setVec3(const std::string&, const glm::vec3 &) const;
        void setVec4(const std::string&, const glm::vec4 &) const;
        void setMat4(const std::string&, const glm::mat4 &) const;

        std::string name;
    private:
        unsigned int m_UIProgramID;
        unsigned int CreateShader(const char*, unsigned int);
};

#endif //SHADER_H