#include <vector>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include "vananagui/base.h"
#include "vananagui/VananaGUI.h"
#include "vananagui/platform/Clock.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/platform/Window.h"
#include "vananagui/platform/DisplayDevice.h"
#include "vananagui/ui/UIElement.h"
#include "vananagui/renderer/ShaderProgram.h"
#include "vananagui/renderer/SpriteRenderer.h"
#include "vananagui/renderer/Fader.h"
#include "vananagui/Vanme.h"



static const char* s_shaderRectangleVert = \
    "#version 120\n"\
    "uniform vec4 uPosition;"\
    "uniform vec4 uColor;"\
    "uniform mat4 uMatrix;"\
    "attribute vec2 aVertices;"\
    "void main(void) {"\
    "    gl_Position = uMatrix * vec4(uPosition.xy + aVertices * uPosition.zw, 0.0, 1.0);"\
    "}";
static const char* s_shaderRectangleFrag = \
    "#version 120\n"\
    "uniform vec4 uColor;"\
    "void main(void) {"\
    "    gl_FragColor.rgba = uColor;"\
    "}";

static const char* s_shaderTextureVert = \
    "#version 120\n"\
    "uniform mat4 uMatrix;"\
    "uniform vec2 uSize;"\
    "attribute vec2 aVertices;"\
    "attribute vec2 aUVs;"\
    "void main(void) {"\
    "    gl_Position = uMatrix * vec4(uSize * aVertices, 0.0, 1.0);"\
    "    gl_TexCoord[0].st = aUVs;"\
    "}";
static const char* s_shaderTextureFrag = \
    "#version 120\n"\
    "uniform sampler2D uSampler;"\
    "void main(void) {"\
    "    gl_FragColor.rgba = texture2D(uSampler, gl_TexCoord[0].st);"\
    "}";



VananaGUI::VananaGUI(AEffEditor* editor, void* hwnd, const char* title, glm::ivec2 size) {
    Van::set(this);

    this->editor = editor;

    this->clock = new Clock();
    this->wnd = new Window(hwnd, title, size);
    this->mouse = new Mouse();
    this->display = new DisplayDevice(this->wnd);
    this->display->initGL();

    this->shaderTexture = new ShaderProgram();
    this->shaderTexture->addStage(s_shaderTextureVert, VERTEX);
    this->shaderTexture->addStage(s_shaderTextureFrag, FRAGMENT);
    this->shaderTexture->link();
    this->shaderRectangle = new ShaderProgram();
    this->shaderRectangle->addStage(s_shaderRectangleVert, VERTEX);
    this->shaderRectangle->addStage(s_shaderRectangleFrag, FRAGMENT);
    this->shaderRectangle->link();

    this->projectionMatrix = glm::ortho(0.0f, (float)this->wnd->getWidth(), (float)this->wnd->getHeight(), 0.0f, -1.0f, 1.0f);

    this->spriteRenderer = new SpriteRenderer();
    this->rectangleRenderer = new RectangleRenderer();

    this->fader = new Fader();
    this->fader->fade();
}

VananaGUI::~VananaGUI() {
    if (this->display) this->display->destroy();
    SAFE_DELETE_PTR(this->wnd);this->wnd=NULL;
}

void VananaGUI::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    this->shaderRectangle->use();
    this->shaderRectangle->uniformMat4f(
        this->shaderRectangle->locateUniform("uMatrix"),
        this->getProjectionMatrix()
    );

    for (auto e : this->elements)
        e->render();

    this->fader->render();

    this->display->swap();
    m_shouldDraw = false;
}

void VananaGUI::insertUIElement(UIElement* element) {
    this->elements.push_back(element);
}

void VananaGUI::handleMouseButton(int btn, bool state) {
    if (MOUSE_BUTTON_LEFT == btn)
        this->mouse->setGrabbed(state);

    for (auto e : this->elements)
        e->handleMouseButton(this->mouse, btn, state);
}

void VananaGUI::handleMouseMotion(long xRel, long yRel) {
    for (auto e : this->elements)
        e->handleMouseMotion(this->mouse, xRel, yRel);
}

void VananaGUI::handleMouseWheel(bool dir) {
    for (auto e : this->elements)
        e->handleMouseWheel(dir);
}


Window& VananaGUI::getWindow() {
    return *this->wnd;
}

Mouse& VananaGUI::getMouse() {
    return *this->mouse;
}

Clock& VananaGUI::getClock() {
    return *this->clock;
}

AEffEditor* VananaGUI::getEditor() {
    return this->editor;
}

glm::mat4 VananaGUI::getProjectionMatrix() {
    return this->projectionMatrix;
}

bool VananaGUI::shouldDraw() {
    return (m_shouldDraw || !this->fader->isDone());
    // if (this->fader->isDone());
    //     return false;
    // this->clock->tick();
    // return true;
}
