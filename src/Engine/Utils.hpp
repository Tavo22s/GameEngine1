#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>

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

    inline std::string Path = "";
    inline unsigned int Width = 0;
    inline unsigned int Height = 0;
}

#endif //UTILS_H