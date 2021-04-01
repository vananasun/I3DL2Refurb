#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>



class Mouse;
class UIElement {
public:
    virtual void render() {};
    virtual void handleMouseButton(Mouse* mouse, int btn, bool state) {};
    virtual void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0) {};
    virtual void handleMouseWheel(bool dir) {};
    virtual UIElement* setScale(glm::vec2 scale) {
        this->scale = scale;
        this->calculateBounds();
        return this;
    };
    virtual UIElement* setPosAndSize(glm::vec2 pos, glm::vec2 size) {
        this->pos = pos;
        m_size = size;
        this->calculateBounds();
        return this;
    };
    virtual UIElement* setPos(glm::vec2 pos) {
        this->pos = pos;
        this->calculateBounds();
        return this;
    };
    virtual UIElement* addTexOffset(glm::vec2 offset) {
        this->pos += offset;
        return this;
    }
    virtual void calculateBounds() {
        this->bounds = {
            (int)this->pos.x,
            (int)this->pos.y,
            (int)this->pos.x + (int)m_size.x,
            (int)this->pos.y + (int)m_size.y,
        };
    };

    glm::vec2 pos;
    glm::vec2 m_size;
    glm::vec2 scale = { 1.0, 1.0 };
    glm::ivec4 bounds;
};
