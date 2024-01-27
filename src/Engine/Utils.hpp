#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

namespace Utils
{
    template<typename T>
    inline std::string to_string(T v)
    {
        std::ostringstream osstr;
        osstr << v;
        return osstr.str();
    }

    inline std::string Ex(std::string str, int n0, int N)
    {
        str[N] = '\0';
        return &str[n0];
    }

    inline std::string attach_strings(std::string str1, std::string str2)
    {
        std::ostringstream osstr;
        osstr << str1 << str2;
        return osstr.str();
    }

    #ifdef __linux__
        inline void FixPathToLinux(std::string &path)
        {
            for(std::size_t i=0; i < path.size(); i++)
                if(path[i] == '\\')
                    path[i] = '/';
        }
    #endif

    inline std::string Path = "";
    inline unsigned int Width = 0;
    inline unsigned int Height = 0;
}

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
    glm::mat4 to;


    to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
    to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
    to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
    to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;

    return to;
}

#endif //UTILS_H