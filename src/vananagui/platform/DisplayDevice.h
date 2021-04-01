#pragma once
#include "vananagui/platform/GLContext.h"

class Window;

class DisplayDevice {
private:
    GLContext* glContext;
    Window* wnd;
public:
    DisplayDevice(Window* wnd);
    void destroy();
    void swap();
    void initGL();
};
