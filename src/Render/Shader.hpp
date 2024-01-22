#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glm/glm.hpp>

class Shader
{
    public:
        ~Shader();

        void use() const;
        void setBool(const std::string&, const bool&) const;
        void setInt(const std::string&, const int&) const;
        void setFloat(const std::string&, const float&) const;
        void setVec2(const std::string&, const glm::vec2 &) const;
        void setVec3(const std::string&, const glm::vec3 &) const;
        void setVec4(const std::string&, const glm::vec4 &) const;
        void setMat4(const std::string&, const glm::mat4 &) const;

        friend Shader *LoadShader(const char*, const char*, const char*);
        friend Shader *LoadShader(const char*, const char*);

        std::string name;
    private:
        Shader();

        bool InitShader(const char*, const char*, const char* = nullptr);
        unsigned int m_UIProgramID;
        unsigned int CreateShader(const char*, unsigned int);
};

Shader *LoadShader(const char*, const char*, const char*);
Shader *LoadShader(const char*, const char*);

#endif //SHADER_H