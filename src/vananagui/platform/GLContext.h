#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/Window.h"
class GLContext {
private:
    HGLRC ctx;
public:
    void create(Window* wnd);
    void destroy();
};
