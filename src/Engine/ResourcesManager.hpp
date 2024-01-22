#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include <vector>
#include "../Render/Shader.hpp"

class ResourcesManager
{
    public:
        ResourcesManager();
        ~ResourcesManager();

        bool Init();

        Shader *GetShader(const std::string &name) const;
    private:
        template<class T>
        void insertionSort(std::vector<T>& lista);

        template<class T>
        int binarySearch(const std::vector<T> &lista, const std::string &name) const;

        std::vector<Shader*> m_vShaders;
};

#endif //RESOURCESMANAGER_H