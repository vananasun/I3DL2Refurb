#pragma once
#include "vananagui/ui/UIElement.h"

class Texture;
class UISliderHorizontal : public UIElement {
public:
    UISliderHorizontal(int paramID, const char* imgFilled, bool invert = false);
    void render();
    void handleMouseButton(Mouse* mouse, int btn, bool state);
    void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0);
    void calculateBounds();


    float value;
    bool hovered = false, grabbed = false, invert;

    int paramID;
    Texture* texFilled;
};
