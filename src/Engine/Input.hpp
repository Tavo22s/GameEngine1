#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

enum
{
    KEY_NONE = 0,
    KEY_DOWN = 1,
    KEY_UP = 2,
    KEY_PRESS = 3,
};

enum KeyCode
{
    KNULL = 0,SPACE = 32, COMMA = 44, MINUS = 45, K0 = 48, K1 = 49, K2 = 50, K3 = 51, K4 = 52, K5 = 53, K6 = 54, K7 = 55, K8 = 56, K9 = 57, A = 65, B = 66, C = 67,
    D = 68, E = 69, F = 70, G = 71, H = 72, I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86,
    W = 87, X = 88, Y = 89, Z = 90, ESCAPE = 256, ENTER = 257, TAB = 258, RIGHT = 262, LEFT = 263, DOWN = 264, UP = 265, F1 = 290, F2 = 291, F3 = 292,
    F4 = 293, F5 = 294, F6 = 295, F7 = 296, F8 = 297, F9 = 298, F10 = 299, F11 = 300, F12 = 301, LEFT_SHIFT = 340, LEFT_CONTROL = 341, LEFT_ALT = 342,
    RIGHT_SHITF = 344, RIGHT_CONTROL = 345, RIGHT_ALT = 346
};

class __INPUT__
{
    public:
        unsigned int keys[1024];
        unsigned int buttons[5];

        glm::vec2 mousePosition;

        __INPUT__();
        ~__INPUT__();


        void Update();

        inline bool GetButton(unsigned int key) const
        {
            return buttons[key];
        }

        inline bool GetButtonDown(unsigned int key) const
        {
            return buttons[key] == KEY_DOWN;
        }

        inline bool GetButtonUp(unsigned int key) const
        {
            return buttons[key] == KEY_UP;
        }

        inline bool GetKeyDown(unsigned int key) const
        {
            return keys[key] == KEY_DOWN;
        }

        inline bool GetKeyPress(unsigned int key) const
        {
            return keys[key] == KEY_PRESS;
        }

        inline bool GetKeyUp(unsigned int key) const
        {
            return keys[key] == KEY_UP;
        }

        void SetKeyUp(unsigned int key);

        void SetKeyDown(unsigned int key);

    private:
        std::vector<unsigned int> m_vUpList;
        std::vector<unsigned int> m_vDownList;
};

inline __INPUT__ Input;

#endif //INPUT_H