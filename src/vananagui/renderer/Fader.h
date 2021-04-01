#pragma once
class Fader {
private:
    float alpha = 1.0f;
public:
    void fade();
    void render();
    bool isDone() { return this->alpha <= 0.0f; };
};
