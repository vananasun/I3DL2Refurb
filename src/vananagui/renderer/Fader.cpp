#include "vananagui/platform/Clock.h"
#include "vananagui/renderer/RectangleRenderer.h"
#include "vananagui/renderer/Fader.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"

void Fader::fade() {
    this->alpha = 1.0f;
}

void Fader::render() {
    if (this->isDone()) return;

    Van::me().rectangleRenderer->render(
        {0, 0},
        {Van::me().getWindow().getWidth(), Van::me().getWindow().getHeight()},
        {0, 0, 0, alpha}
    );

    Van::me().getClock().tick();
    this->alpha -= 0.05f * (Van::me().getClock().getElapsedSeconds() * 15);
}
