#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glm/ext/vector_int2.hpp>

class Window {
private:
    HWND hwnd;
    HDC hdc;
    int width;
    int height;
    const char* windowClassName;
    long m_cursor;
    bool m_cursorLocked = false;
    bool m_cursorResetting = false;
public:
    Window(void* hwnd, const char* title, glm::ivec2 dimensions);
    ~Window();
    void show(bool show = true);
    HWND getHwnd();
    HDC getHdc();
    int getWidth();
    int getHeight();
    void setCursor(long cursor);
    long getCursor();
    void setCursorLocked(bool locked);
    void resetCursor(bool reset = true);
    bool isCursorResetting() { return m_cursorResetting; };
};

#define CURSOR_ARROW 32512
#define CURSOR_HAND 32649
#define CURSOR_VERTICAL 32645

#define CURSOR_STATE_LOCKED 1
#define CURSOR_STATE_RESET  2
