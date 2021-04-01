#pragma once
#include "vananagui/ui/UIElement.h"
#include "vananagui/renderer/Texture.h"

class UIImage : public UIElement {
public:
    UIImage(const char* image, glm::vec2 pos = {0, 0});
    void render();

    Texture* tex;
};
