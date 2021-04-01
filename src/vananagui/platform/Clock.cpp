#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/Clock.h"
#include "vananagui/base.h"

Clock::Clock() {
    QueryPerformanceFrequency((LARGE_INTEGER*)&this->frequency);
    this->lastTime = this->getTime();
}

void Clock::tick() {
    double newTime = this->getTime();
    this->elapsedSeconds = (double)(newTime - this->lastTime);
    this->lastTime = newTime;
}

double Clock::getTime() {
    LONGLONG ticks;
    QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
    return ticks / (double)this->frequency;
}

double Clock::getElapsedSeconds() {
    return this->elapsedSeconds;
}
