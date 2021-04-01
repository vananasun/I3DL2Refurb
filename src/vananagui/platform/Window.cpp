#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "vananagui/platform/Window.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/VananaGUI.h"
#include "vananagui/Vanme.h"



extern void* hInstance;

static const int s_windowStyle = WS_CHILD | WS_VISIBLE;




static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    VananaGUI* vananagui = reinterpret_cast<VananaGUI*>(GetWindowLongPtr(hwnd, 0));

    switch (msg) {
    // handle close button
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        return 0;

    // focus
    case WM_SETFOCUS:
        return 0;
    case WM_KILLFOCUS:
        return 0;

    // input
    // case WM_INPUT:
    //     vananagui->getMouse().handle(reinterpret_cast<HRAWINPUT>(lParam));
    //     return 0;
    case WM_LBUTTONDOWN:
        vananagui->handleMouseButton(MOUSE_BUTTON_LEFT, true);
        vananagui->render();
        return 0;
    case WM_LBUTTONUP:
        vananagui->handleMouseButton(MOUSE_BUTTON_LEFT, false);

        // We might have to check again whether the mouse hovers over a knob
        if (vananagui->getWindow().isCursorResetting()) {
            vananagui->getWindow().setCursor(CURSOR_ARROW);
            vananagui->handleMouseMotion(0, 0); // simulate mouse move to do hover checks
            vananagui->getWindow().resetCursor(false);
        }

        vananagui->render();
        return 0;

    case WM_MOUSEMOVE:
        vananagui->getMouse().updatePosition();
        glm::vec2 vel = vananagui->getMouse().getLowResVelocity();
        vananagui->handleMouseMotion(vel.x,vel.y);
        vananagui->render();
        return 0;

    case WM_MOUSEWHEEL:
        vananagui->handleMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) > 0);
        vananagui->render();
        return 0;

    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(vananagui->getWindow().getCursor())));
        return 0;


    default:
        // call Windows' default window proc
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}


void Window::setCursor(long cursor) {
    if (m_cursorLocked) return; // locked means we cannot change the cursor

    m_cursor = cursor;
    m_cursorResetting = false; // always cancel reset operation
    PostMessage(this->hwnd, WM_SETCURSOR, 0, 0);
}

long Window::getCursor() { return m_cursor; }

void Window::setCursorLocked(bool locked) {
    m_cursorLocked = locked;
}

void Window::resetCursor(bool reset) {
    m_cursorResetting = reset;
}

Window::Window(void* hwnd, const char* title, glm::ivec2 dimensions) {
    // define the window class
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_GLOBALCLASS; //CS_CLASSDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 1 * sizeof(LONG_PTR);
    wc.hInstance     = (HINSTANCE)hInstance;
    wc.hIcon         = LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = title;
    wc.hIconSm       = LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    // register it
    if (!RegisterClassEx(&wc)) {
        #ifdef DEBUG
        printf("Failed to register window class (does it already exist?) (error %lu).\n", GetLastError());
        #endif
    }
    this->windowClassName = title;

    // create window
    this->width = dimensions.x;
    this->height = dimensions.y;
    this->hwnd = CreateWindow(
        title, title,
        s_windowStyle,
        0, 0,
        width, height,
        static_cast<HWND>(hwnd), NULL,
        (HINSTANCE)hInstance, NULL
    );
    #ifdef DEBUG
    if (!this->hwnd) puts("Failed to create window.");
    #endif

    // store the window handle
    this->hdc = GetDC(this->hwnd);
    #ifdef DEBUG
    if (!this->hdc) puts("Failed to get DC.");
    #endif
    SetWindowLongPtr(this->hwnd, 0, reinterpret_cast<LONG_PTR>(&Van::me()));
}

Window::~Window() {
    DestroyWindow(this->hwnd);
    UnregisterClassA(this->windowClassName, (HINSTANCE)hInstance);
}

void Window::show(bool show) {
    ShowWindow(this->hwnd, show * SW_SHOW);
}

HWND Window::getHwnd() { return this->hwnd; }
HDC Window::getHdc() { return this->hdc; }
int Window::getWidth() { return this->width; }
int Window::getHeight() { return this->height; }
