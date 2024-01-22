#ifndef TIME_H
#define TIME_H

struct _TIME__
{
    float deltaTime = .0f;
    float time = .0f;
    float lastFrame = .0f;
};

inline _TIME__ Time;

#endif //TIME_H