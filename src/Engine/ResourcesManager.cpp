#include "ResourcesManager.hpp"

ResourcesManager::ResourcesManager()
{
}

ResourcesManager::~ResourcesManager()
{
}

bool ResourcesManager::Init()
{
    
    return true;
}

Shader *ResourcesManager::GetShader(const std::string &name) const
{
    int id = binarySearch(m_vShaders, name);
    if(id != -1)
        return m_vShaders[id];
    return nullptr;
} 

template<class T>
void ResourcesManager::insertionSort(std::vector<T>& lista)
{
    unsigned int n = lista.size();
    for(unsigned int i=1; i<n; i++)
    {
        std::string clave = lista[i]->name;
        int j = i - 1;
        while(j >= 0 && lista[j] > clave)
        {
            lista[j+1] = lista[j];
            j = j - 1;
        }
        lista[j + 1] = clave;
    }
}

template<class T>
int ResourcesManager::binarySearch(const std::vector<T> &lista, const std::string &name) const
{
    int left = 0;
    int right = lista.size() - 1;

    while(left <= right)
    {
        int mid = left + (right - left) / 2;

        if(lista[mid]->name == name)
            return mid;

        if(lista[mid]->name > name)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;
}