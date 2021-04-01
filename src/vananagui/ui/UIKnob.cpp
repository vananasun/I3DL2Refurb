#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/renderer/Texture.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/ui/UIKnob.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"
#define M_PI 3.14159265358979323846264338327950288



UIKnob::UIKnob(int paramID, const char* img, const char* imgHover, const char* imgHold) {
    this->paramID = paramID;
    this->tex = new Texture(img);
    if (imgHover) this->texHover = new Texture(imgHover);
    if (imgHold) this->texHold = new Texture(imgHold);
}

void UIKnob::setOrigin(glm::vec2 origin) {
    this->origin = origin;
}

void UIKnob::calculateBounds() {
    this->bounds = {
        static_cast<int>(this->pos.x),
        static_cast<int>(this->pos.y),
        static_cast<int>(this->pos.x + this->knobSize.x),
        static_cast<int>(this->pos.y + this->knobSize.y)
    };
}

void UIKnob::render() {
    float value = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    Texture* renderTex = (this->grabbed && this->texHold) ? this->texHold :
                         (this->hovered && this->texHover) ? this->texHover :
                         this->tex;
    int index = (int)(value * this->frames);
    if (index == this->frames) index--;
    Van::me().spriteRenderer->renderIndexFromStrip(*renderTex, this->pos, index, this->frames, this->knobSize);
}

void UIKnob::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn != MOUSE_BUTTON_LEFT) return;

    // Handle release; Cursor should reset
    if (this->grabbed && !state) {
        // Determine if we are still hovering the button or not
        Van::me().getWindow().setCursorLocked(false);
        if (mouse->inRect(this->bounds))
            Van::me().getWindow().setCursor(CURSOR_HAND);
        else {
            Van::me().getWindow().resetCursor();
        }

        // Un-grab
        // AudioEffectX* effectx = static_cast<AudioEffectX*>(Van::me().getEditor()->getEffect());
        // effectx->endEdit(this->paramID);
        this->grabbed = false;
        return;
    }

    // Check whether we clicked the button
    this->grabbed = (state && mouse->inRect(this->bounds));
    if (this->grabbed) {
        // AudioEffectX* effectx = static_cast<AudioEffectX*>(Van::me().getEditor()->getEffect());
        // effectx->beginEdit(this->paramID);
        Van::me().getWindow().setCursor(CURSOR_VERTICAL);
        Van::me().getWindow().setCursorLocked(true);
    }
}

void UIKnob::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    if (!this->grabbed) {
        bool wasHovered = this->hovered;
        this->hovered = mouse->inRect(this->bounds);
        if (this->hovered) {
            Van::me().getWindow().setCursor(CURSOR_HAND);
        } else if (wasHovered) {
            Van::me().getWindow().setCursor(CURSOR_ARROW);
        }
        return;
    }

    float value = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    value = MAX(MIN(value - (yRel * 1.4 / 270.0), 1.0), 0.0);
    Van::me().getEditor()->getEffect()->setParameterAutomated(this->paramID, value);
}


void UIKnob::handleMouseWheel(bool dir) {
    if (!(this->hovered || this->grabbed)) return;
    const float WHEEL_MULTIPLIER = 1.0 / 15;
    float value = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    value = MAX(MIN(value + (WHEEL_MULTIPLIER * (dir * 2 - 1)), 1.0), 0.0);
    Van::me().getEditor()->getEffect()->setParameterAutomated(this->paramID, value);
}
