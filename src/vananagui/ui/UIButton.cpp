#include "vananagui/ui/UIButton.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"

UIButton::UIButton(const char* imgOff, const char* imgOn, const char* imgHover) {
    if (imgOff) m_texOff = new Texture(imgOff);
    if (imgOn) m_texOn = new Texture(imgOn);
    if (imgHover) m_texHover = new Texture(imgHover);
}

void UIButton::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    bool wasHovered = m_hovered;
    m_hovered = mouse->inRect(this->bounds);
    if (m_hovered) {
        Van::me().getWindow().setCursor(CURSOR_HAND);
        if (m_texHover) Van::me().render(); // for potential hover texture
    } else if (wasHovered)
        Van::me().getWindow().setCursor(CURSOR_ARROW);
}

void UIButton::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn == MOUSE_BUTTON_LEFT && state && m_hovered) {
        m_onclick(this);
    }
}

void UIButton::render() {
    // printf("state is: %i, hover: %i, hovered: %i\n", (int)m_state, (int)m_hovered, (int)(Van::me().getMouse().inRect(this->bounds)));
    if (m_state) {
        if (m_texOn) Van::me().spriteRenderer->render(*m_texOn, this->pos);
    } else if (m_hovered) {
        if (m_texHover) Van::me().spriteRenderer->render(*m_texHover, this->pos);
    } else {
        if (m_texOff) Van::me().spriteRenderer->render(*m_texOff, this->pos);
    }
}
