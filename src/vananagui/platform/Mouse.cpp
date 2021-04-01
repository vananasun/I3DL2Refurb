/**
 * Dubnium DirectInput mouse written by Maxim van Dijk.
 * Edited for C++.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/Mouse.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"



/*******************************************************************************
Local functions
*******************************************************************************/
static void      deviceRegister(RAWINPUTDEVICE* device);
static UINT      messageGetSize(HRAWINPUT header);
static RAWINPUT* messageReceive(HRAWINPUT header, UINT size);

/*******************************************************************************
DESCRIPTION:
    Register the RawInputDevice struct so raw input gets routed through to the
    window procedure with WM_INPUT messages.
ARGUMENTS:
    device - RAWINPUTDEVICE struct to register.
*******************************************************************************/
static void deviceRegister(RAWINPUTDEVICE* device) {
    if (!RegisterRawInputDevices(device, 1, sizeof(device[0]))) {
        #ifdef DEBUG
        puts("Failed to register mouse input device.");
        #endif
    }
}

/*******************************************************************************
DESCRIPTION:
    Get the raw input device message size.
ARGUMENTS:
    header - Raw input header.
RETURNS:
    Retrieved message size.
*******************************************************************************/
static UINT messageGetSize(HRAWINPUT header) {
    // get size
    UINT size;
    UINT result = GetRawInputData(
        header,                // raw input struct
        RID_INPUT,             // command flag
        NULL,                  // structure data
        &size,                 // size
        sizeof(RAWINPUTHEADER) // input header size
    );
    // check for errors
    #ifdef DEBUG
    if (-1 == result)
        puts("Failed to get raw input message size.");
    #endif
    return size;
}

/*******************************************************************************
DESCRIPTION:
    Retrieve the raw input device message.
ARGUMENTS:
    header - Raw input header.
    size - Expected message size.
RETURNS:
    Pointer to a RAWINPUT message. NOTE: Returns allocated buffer.
*******************************************************************************/
static RAWINPUT* messageReceive(HRAWINPUT header, UINT size) {
    // get message data
    LPBYTE* data = static_cast<LPBYTE*>(malloc(size));
    UINT result = GetRawInputData(
        header,                // raw input struct
        RID_INPUT,             // command flag
        data,                  // structure data
        &size,                 // size
        sizeof(RAWINPUTHEADER) // input header size
    );
    // check whether returned size is the same as the expected size
    #ifdef DEBUG
    if (result != size)
        puts("GetRawInputData returned an unexpected message size.");
    #endif
    return (RAWINPUT*)data;
}



/*******************************************************************************
Global functions
*******************************************************************************/

/*******************************************************************************
DESCRIPTION:
    Handle a raw input message and send events for specific messages.
ARGUMENTS:
    message - Raw input message.
*******************************************************************************/
void Mouse::messageHandle(RAWINPUT* message) {
    // check for correct message type
    if (RIM_TYPEMOUSE != message->header.dwType)
        return;
    // check buttons
    USHORT flags = message->data.mouse.usButtonFlags;
    #define F(flag) if(flags&flag)
    F(RI_MOUSE_LEFT_BUTTON_DOWN)
        Van::me().handleMouseButton(MOUSE_BUTTON_LEFT, true);
    F(RI_MOUSE_LEFT_BUTTON_UP)
        Van::me().handleMouseButton(MOUSE_BUTTON_LEFT, false);
    F(RI_MOUSE_RIGHT_BUTTON_DOWN)
        Van::me().handleMouseButton(MOUSE_BUTTON_RIGHT, true);
    F(RI_MOUSE_RIGHT_BUTTON_UP)
        Van::me().handleMouseButton(MOUSE_BUTTON_RIGHT, false);
    F(RI_MOUSE_MIDDLE_BUTTON_DOWN)
        Van::me().handleMouseButton(MOUSE_BUTTON_MIDDLE, true);
    F(RI_MOUSE_MIDDLE_BUTTON_UP)
        Van::me().handleMouseButton(MOUSE_BUTTON_MIDDLE, false);
    F(RI_MOUSE_WHEEL)
        Van::me().handleMouseWheel(message->data.mouse.usButtonData == 0xFF88);
    #undef F
    // position updates
    // LONG xRel = message->data.mouse.lLastX;
    // LONG yRel = message->data.mouse.lLastY;
    // if (xRel != 0 || yRel != 0)
    //     this->vananagui->handleMouseMotion(xRel, yRel);
}

Mouse::Mouse() {
    // // create device struct
    // #ifndef HID_USAGE_PAGE_GENERIC
    // #define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
    // #endif
    // #ifndef HID_USAGE_GENERIC_MOUSE
    // #define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
    // #endif
    // RAWINPUTDEVICE device[1];
    // device[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    // device[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
    // device[0].dwFlags     = RIDEV_INPUTSINK;
    // device[0].hwndTarget  = Van::me().getWindow().getHwnd();
    // // register mouse input device
    // deviceRegister(device);
    // reset motion vectors
    this->updatePosition();
}

void Mouse::handle(HRAWINPUT header) {
    // if (!this->grabbed) return;
    UINT size = messageGetSize(header);
    RAWINPUT* message = messageReceive(header, size);
    this->messageHandle(message);
    free(message);
}

void Mouse::setGrabbed(bool grabbed) {
    this->grabbed = grabbed;
    // ShowCursor(!grabbed);
    if (grabbed)
        SetCapture(Van::me().getWindow().getHwnd());
    else
        ReleaseCapture();
}

bool Mouse::isGrabbed() {
    return this->grabbed;
}

void Mouse::updatePosition() {
    POINT p;
    GetCursorPos(&p);
    HWND hwnd = Van::me().getWindow().getHwnd();
    ScreenToClient(hwnd, &p);

    this->posLowResPrev = this->posLowRes;
    this->posLowRes = glm::vec2(p.x, p.y);
}

glm::vec2 Mouse::getLowResVelocity() {
    return { this->posLowRes.x - this->posLowResPrev.x,
             this->posLowRes.y - this->posLowResPrev.y };
}

bool Mouse::inRect(glm::ivec4 r) {
    return (this->posLowRes.x >= r.x && this->posLowRes.x < r.z &&
            this->posLowRes.y >= r.y  && this->posLowRes.y < r.w);
}
