#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>



class Mouse {
private:
    void messageHandle(RAWINPUT* message);

public:
    /**
     * @constructor
     * Register the raw mouse input handler.
     */
    Mouse();

    /**
     * Handle a WM_INPUT lParam raw input message.
     *
     * @param HRAWINPUT header
     */
    void handle(HRAWINPUT header);

    /**
     * Capture or release the mouse by the window.
     *
     * @param bool grabbed
     */
    void setGrabbed(bool grabbed);

    /**
     * @return bool - Whether the mouse is grabbed by the window.
     */
    bool isGrabbed();

    /**
     * Update the low resolution position, which are the coordinates relative to
     * the window.
     *
     * @param HWND hwnd
     */
    void updatePosition();

    /**
     * Calculates the low resolution motion vector of the mouse.
     *
     * glm::vec2 velocity
     */
    glm::vec2 getLowResVelocity();

    /**
     * Checks whether the mouse is currently inside the given rectangle.
     *
     * @param glm::ivec4 r
     *
     * @return bool
     */
    bool inRect(glm::ivec4 r);



    bool grabbed = true; // whether the mouse is grabbed by the window.
    glm::vec2 posLowRes;
    glm::vec2 posLowResPrev;
};

#define MOUSE_BUTTON_LEFT   0
#define MOUSE_BUTTON_RIGHT  1
#define MOUSE_BUTTON_MIDDLE 2
