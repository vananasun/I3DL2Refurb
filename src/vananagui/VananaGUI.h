#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "vananagui/platform/DisplayDevice.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/ui/UIImage.h"
#include "vananagui/ui/UISliderHorizontal.h"
#include "vananagui/ui/UIKnob.h"
#include "vananagui/ui/UIButton.h"
#include "vananagui/renderer/ShaderProgram.h"
#include "vananagui/renderer/SpriteRenderer.h"
#include "vananagui/renderer/RectangleRenderer.h"
#include "vananagui/renderer/Fader.h"
#include "vananagui/base.h"

class AEffEditor;
class Window;
class Clock;

class VananaGUI {
private:
    std::vector<UIElement*> elements;
    DisplayDevice* display;
    Window* wnd;
    Mouse* mouse;
    Clock* clock;
    AEffEditor* editor;
    Fader* fader;
    glm::vec2 scale = {1.0, 1.0};
    bool m_shouldDraw = false;

public:
    /**
     * Initialize VananaGUI.
     *
     * @param AEffEditor* editor VST Effect editor.
     * @param void*       hwnd   Parent window pointer.
     * @param const char* title  Title of the GUI window.
     * @param glm::ivec2  size   Dimensions of the GUI window.
     */
    VananaGUI(AEffEditor* editor, void* hwnd, const char* title, glm::ivec2 size);

    /**
     * Cleans up the VananaGUI object.
     */
    ~VananaGUI();

    /**
     * Cause the window to be rendered.
     */
    void render();

    /**
     * Insert a UI element into the GUI.
     *
     * @param UIElement* element
     */
    void insertUIElement(UIElement* element);

    /**
     * @param int  btn
     * @param bool state
     */
    void handleMouseButton(int btn, bool state);

    /**
     * Triggers a mouse motion event.
     *
     * @param long xRel (optional)
     * @param long yRel (optional)
     */
    void handleMouseMotion(long xRel = 0, long yRel = 0);

    /**
     * @param bool dir
     */
    void handleMouseWheel(bool dir);

    /**
     * @return DisplayDevice&
     */
    DisplayDevice& getDisplayDevice();

    /**
     * @return Window&
     */
    Window& getWindow();

    /**
     * @return bool
     */
    bool isWindowOpen() { return 0 != this->wnd; };

    /**
     * @return Mouse&
     */
    Mouse& getMouse();

    /**
     * @return Clock&
     */
    Clock& getClock();

    /**
     * @return AEffEditor*
     */
    AEffEditor* getEditor();

    /**
     * @return glm::mat4
     */
    glm::mat4 getProjectionMatrix();

    /**
     * @return bool dirty
     */
    bool shouldDraw();

    /**
     * Marks the screen as dirty.
     */
    void redraw() { m_shouldDraw = true; }



    ShaderProgram* shaderRectangle;
    ShaderProgram* shaderTexture;
    glm::mat4 projectionMatrix;
    SpriteRenderer* spriteRenderer;
    RectangleRenderer* rectangleRenderer;
};
