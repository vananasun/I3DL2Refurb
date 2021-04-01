#pragma once
#include <functional>
#include "vananagui/ui/UIElement.h"

class Texture;
class UIButton : public UIElement {
public:
    UIButton(const char* imgOff = 0, const char* imgOn = 0, const char* imgHover = 0);
    void setClickHandler(std::function<void(UIButton*)> onclick) { m_onclick = onclick; };
    void setState(bool state) { m_state = state; };
    bool getState() { return m_state; };
    void handleMouseMotion(Mouse* mouse, long xRel, long yRel);
    void handleMouseButton(Mouse* mouse, int btn, bool state);
    void render();
private:
    std::function<void(UIButton*)> m_onclick;
    bool m_state = false;
    bool m_hovered = false;
    Texture* m_texOff = 0;
    Texture* m_texOn = 0;
    Texture* m_texHover = 0;
};
