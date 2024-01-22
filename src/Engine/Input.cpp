#include "Input.hpp"

__INPUT__::__INPUT__()
{
    for(int i = 0; i < 1024; i++)
        keys[i] = KEY_NONE;

    for(int i=0; i < 5; i++)
        buttons[i] = KEY_NONE;
}

__INPUT__::~__INPUT__()
{
}

void __INPUT__::Update()
{
    for(auto key: m_vUpList)
        keys[key] = KEY_NONE;
    m_vUpList.clear();

    for(auto key: m_vDownList)
        keys[key] = KEY_PRESS;
    m_vDownList.clear();

    for(int i=0; i<5; i++)
        if(buttons[i] == KEY_UP)
            buttons[i] = KEY_NONE;
}

void __INPUT__::SetKeyUp(unsigned int key)
{
    keys[key] = KEY_UP;
    m_vUpList.push_back(key);
}

void __INPUT__::SetKeyDown(unsigned int key)
{
    keys[key] = KEY_DOWN;
    m_vDownList.push_back(key);
}