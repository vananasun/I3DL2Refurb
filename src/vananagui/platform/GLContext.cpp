#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLEW_STATIC
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glext.h>
#include <stdio.h>
#include "vananagui/platform/Window.h"
#include "vananagui/platform/GLContext.h"



void GLContext::create(Window* wnd) {
    // create context
    HDC dc = wnd->getHdc();
    this->ctx = wglCreateContext(dc);

    // make context current
    wglMakeCurrent(dc, this->ctx);

    // verify min. OpenGL version
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    #ifdef DEBUG
    if (!(major >= 3 && minor >= 0)) {
        printf("Outdated OpenGL version %i,%i.\n", major, minor);
    }
    printf("Using OpenGL version %i,%i.\n", major, minor);
    #endif

    // init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    #ifdef DEBUG
    if (GLEW_OK != err)
        printf("GLEW failed to initialise.\n");
    #endif

    // setup GL parameters
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, wnd->getWidth(), wnd->getHeight());
}

void GLContext::destroy() {
    wglMakeCurrent(wglGetCurrentDC(), NULL);
    wglDeleteContext(this->ctx);
}
