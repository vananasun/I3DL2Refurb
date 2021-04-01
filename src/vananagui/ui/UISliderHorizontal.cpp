#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/ui/UISliderHorizontal.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/renderer/ShaderProgram.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"

#define MOISTITY 1.6f

UISliderHorizontal::UISliderHorizontal(int paramID, const char* imgFilled, bool invert) {
    this->invert = invert;
    this->paramID = paramID;

    this->texFilled = new Texture(imgFilled);

    this->value = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    if (this->invert) this->value = 1.0f - this->value;
}

void UISliderHorizontal::calculateBounds() {
    this->bounds = {
        static_cast<int>(this->pos.x),
        static_cast<int>(this->pos.y),
        static_cast<int>(this->pos.x + this->texFilled->width  * this->scale.x),
        static_cast<int>(this->pos.y + this->texFilled->height * this->scale.y)
    };
}

void UISliderHorizontal::render() {
    float v = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    if (this->invert) v = 1.0f - v;
    if (this->hovered || this->grabbed) {
        Van::me().rectangleRenderer->render(
            this->pos,
            { this->texFilled->width, this->texFilled->height },
            { 207 / 255.0, 207 / 255.0, 155 / 255.0, (this->grabbed ? 0.28f : 0.14f) });
    }
    Van::me().spriteRenderer->renderHorizontalBar(*this->texFilled, this->pos, this->scale, v);

}

void UISliderHorizontal::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn != MOUSE_BUTTON_LEFT) return;

    this->grabbed = (state && mouse->inRect(this->bounds));
}

void UISliderHorizontal::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    if (!this->grabbed) {
        this->hovered = mouse->inRect(this->bounds);
        return;
    }

    this->value = MAX(MIN(this->value + (xRel * MOISTITY / this->texFilled->width), 1.0), 0.0);
    Van::me().getEditor()->getEffect()->setParameter(this->paramID, this->invert ? 1.0f - this->value : this->value);
}
