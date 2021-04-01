#pragma once
#include "vananagui/ui/UIElement.h"
#include "vananagui/renderer/Texture.h"

class UIKnob : public UIElement {
public:
    UIKnob(int paramID, const char* img, const char* imgHover = 0, const char* imgHold = 0);
    void render();
    void handleMouseButton(Mouse* mouse, int btn, bool state);
    void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0);
    void handleMouseWheel(bool dir);
    void calculateBounds();
    void setOrigin(glm::vec2 origin); // @TODO: obsolete
    void setStripFrames(int frames) { this->frames = frames; };
    void setSize(glm::vec2 size) { this->knobSize = size; this->calculateBounds(); };



    Texture* tex;
    Texture* texHold = 0;
    Texture* texHover = 0;
    float value;
    bool hovered = false, grabbed = false;
    glm::vec2 origin = {0,0};
    glm::vec2 knobSize = {0,0};

    int paramID;
    int frames = 0;
};
