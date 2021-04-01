#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Clock {
private:
    LONGLONG frequency;
    double lastTime, elapsedSeconds;
public:
    Clock();
    void tick();
    double getTime();
    double getElapsedSeconds();
};
