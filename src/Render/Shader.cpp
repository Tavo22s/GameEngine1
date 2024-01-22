#include <glad/glad.h>
#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(this->m_UIProgramID);
}

bool Shader::InitShader(const char *v_file_path, const char *f_file_path, const char* g_file_path)
{
    unsigned int VertexShaderID = CreateShader(v_file_path, GL_VERTEX_SHADER);
    if(!VertexShaderID)
        return false;
    unsigned int FragmentShaderID = CreateShader(f_file_path, GL_FRAGMENT_SHADER);
    if(!FragmentShaderID)
        return false;
    unsigned int GeometryShaderID;
    if(g_file_path)
    {
        GeometryShaderID = CreateShader(g_file_path, GL_GEOMETRY_SHADER);
        if(!GeometryShaderID)
            return false;
    }
     
    cout<<"Link Program"<<endl;
    m_UIProgramID = glCreateProgram();
    if(!m_UIProgramID)
        return 0;
    glAttachShader(m_UIProgramID, VertexShaderID);
    glAttachShader(m_UIProgramID, FragmentShaderID);
    if(g_file_path)
        glAttachShader(m_UIProgramID, GeometryShaderID);
    glLinkProgram(m_UIProgramID);

    int Result;
    int infoLogLength;

    glGetProgramiv(m_UIProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(m_UIProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0)
    {
        vector<char> ProgramErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(m_UIProgramID, infoLogLength, nullptr, &ProgramErrorMessage[0]);
        cout<<&ProgramErrorMessage[0]<<endl;
        return false;
    }

    glDetachShader(m_UIProgramID, VertexShaderID);
    glDetachShader(m_UIProgramID, FragmentShaderID);
    if(g_file_path)
        glDetachShader(m_UIProgramID, GeometryShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    if(g_file_path)
        glDeleteShader(GeometryShaderID);

    return true;
}

unsigned int Shader::CreateShader(const char *shader, unsigned int type)
{
    unsigned int shaderID = glCreateShader(type);

    string shaderCode;
    ifstream shaderStream(shader, ios::in);
    if(shaderStream.is_open())
    {
        stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else
    {
        cout<<"Error al abrir el archivo"<<endl;
        return 0;
    }

    int Result = GL_FALSE;
    int infoLogLength;
    string _type;
    if(type == GL_VERTEX_SHADER) _type = "Vertex";
    else if(type == GL_FRAGMENT_SHADER) _type = "Fragment";
    else _type = "Geometry";
    cout<<"Compile Shader: "<<_type<<" Shader"<<endl;
    const char *shaderSourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &shaderSourcePointer, nullptr);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0)
    {
        vector<char> shaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
        cout<<&shaderErrorMessage[0]<<endl;
    }

    return shaderID;
}

void Shader::use() const
{
    glUseProgram(m_UIProgramID);
}

void Shader::setBool(const std::string &name, const bool &value) const
{
    this->setInt(name, (int) value);
}

void Shader::setInt(const std::string &name, const int &value) const
{
    glUniform1i(glGetUniformLocation(m_UIProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float &value) const
{
    glUniform1f(glGetUniformLocation(m_UIProgramID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2f(glGetUniformLocation(m_UIProgramID, name.c_str()), value.x, value.y);
}


void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3f(glGetUniformLocation(m_UIProgramID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4f(glGetUniformLocation(m_UIProgramID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_UIProgramID, name.c_str()), 1, false, glm::value_ptr(mat));
}

Shader *LoadShader(const char* vs, const char*fs, const char*gs)
{
    Shader *shader = new Shader();
    
    if(shader->InitShader(vs, fs, gs))
        return shader;
    
    delete shader;
    return nullptr;
}

Shader *LoadShader(const char* vs, const char* fs)
{
    Shader *shader = new Shader();
    if(shader->InitShader(vs, fs, nullptr))
        return shader;
    delete shader;
    return nullptr;
}