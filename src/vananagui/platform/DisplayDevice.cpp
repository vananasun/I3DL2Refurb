#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/Window.h"
#include "vananagui/platform/GLContext.h"
#include "vananagui/platform/DisplayDevice.h"
#include "vananagui/base.h"



static const PIXELFORMATDESCRIPTOR s_PFD = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1, // version

    PFD_SUPPORT_OPENGL | // must support OpenGL
    PFD_DRAW_TO_WINDOW | // must support window drawing
    PFD_DOUBLEBUFFER,    // must support double buffering

    PFD_TYPE_RGBA, // RGBA format
    32, // 32-bit color depth
    0, 0, 0, 0, 0, 0, // skip color bits
    0, // no alpha buffer
    0, // skip shift bit

    0, // no accumulation buffer
    0, 0, 0, 0, // skip accumulation bits

    24, // 24-bit depth buffer
    8, // 8-bit stencil buffer
    0, // no auxiliary buffer
    PFD_MAIN_PLANE, // main drawing layer
    0, // reserved
    0, 0, 0 // skip layer masks
};


DisplayDevice::DisplayDevice(Window* wnd) {
    this->wnd = wnd;
    HDC dc = wnd->getHdc();

    int pf = ChoosePixelFormat(dc, &s_PFD);
    #ifdef DEBUG
    if (!pf) puts("ChoosePixelFormat failed.");
    #endif

    BOOL success = SetPixelFormat(dc, pf, &s_PFD);
    #ifdef DEBUG
    if (!success)
        puts("SetPixelFormat failed.");
    #endif

    this->glContext = new GLContext();
}

void DisplayDevice::destroy() {
    glContext->destroy();
    SAFE_DELETE_PTR(glContext);
}

void DisplayDevice::swap() {
    if (!this->wnd) return;
    SwapBuffers(this->wnd->getHdc());
}

void DisplayDevice::initGL() {
    glContext->create(this->wnd);
}
