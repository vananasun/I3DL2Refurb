#include <vector>
#include <vananagui/VananaGUI.h>
#include <shellapi.h> // hyperlink handler
#include "Refurb.h"
#include "RefurbGUI.h"
#include "I3DL2Reverb.h"
#include "common.h"
#define DEFAULT_KNOB_FRAMES 128



RefurbGUI::RefurbGUI(AudioEffect* effect) : AEffEditor(effect) {
    this->effect = effect;
}

RefurbGUI::~RefurbGUI() {
    this->effect = 0;
}

bool RefurbGUI::getRect(ERect** rect) {
    *rect = &this->rect;
    return false;
}

bool RefurbGUI::isOpen() {
    return m_vananagui->isWindowOpen();
}


bool RefurbGUI::open(void* ptr) {
    m_vananagui = new VananaGUI(this, (HWND)ptr, "I3DL2Refurb by vananaSun", { 510, 317 } );
    this->systemWindow = m_vananagui->getWindow().getHwnd();
    this->rect = {
        0,
        0,
        (VstInt16)m_vananagui->getWindow().getHeight(),
        (VstInt16)m_vananagui->getWindow().getWidth()
    };

    // Background
    m_vananagui->insertUIElement(new UIImage("background_100"));

    auto mix = new UIKnob(I3DL2Reverb::kI3DL2ReverbDryWet, "knobs_big_100");
    mix->setPos({ 398, 200 });
    mix->setSize({ 80, 80 });
    mix->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(mix);

    auto decayTime = new UIKnob(I3DL2Reverb::kI3DL2ReverbDecayTime, "knobs_big_100");
    decayTime->setPos({ 398, 68 });
    decayTime->setSize({ 80, 80 });
    decayTime->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(decayTime);


    float upperY = 64.0f;
    auto reverbDelay = new UIKnob(I3DL2Reverb::kI3DL2ReverbReverbDelay, "knobs_small_100");
    reverbDelay->setPos({ 28, upperY });
    reverbDelay->setSize({ 64, 64 });
    reverbDelay->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(reverbDelay);

    auto early = new UIKnob(I3DL2Reverb::kI3DL2ReverbReflections, "knobs_small_100");
    early->setPos({ 108, upperY });
    early->setSize({ 64, 64 });
    early->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(early);

    auto earlyDelay = new UIKnob(I3DL2Reverb::kI3DL2ReverbReflectionsDelay, "knobs_small_100");
    earlyDelay->setPos({ 190, upperY });
    earlyDelay->setSize({ 64, 64 });
    earlyDelay->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(earlyDelay);

    auto decayHF = new UIKnob(I3DL2Reverb::kI3DL2ReverbDecayHFRatio, "knobs_small_100");
    decayHF->setPos({ 270, upperY });
    decayHF->setSize({ 64, 64 });
    decayHF->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(decayHF);


    float lowerY = 196.0f;
    auto roomHF = new UIKnob(I3DL2Reverb::kI3DL2ReverbRoomHF, "knobs_small_100");
    roomHF->setPos({ 28, lowerY });
    roomHF->setSize({ 64, 64 });
    roomHF->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(roomHF);

    auto refHF = new UIKnob(I3DL2Reverb::kI3DL2ReverbHFReference, "knobs_small_100");
    refHF->setPos({ 108, lowerY });
    refHF->setSize({ 64, 64 });
    refHF->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(refHF);

    auto diffusion = new UIKnob(I3DL2Reverb::kI3DL2ReverbDiffusion, "knobs_small_100");
    diffusion->setPos({ 190, lowerY });
    diffusion->setSize({ 64, 64 });
    diffusion->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(diffusion);

    auto density = new UIKnob(I3DL2Reverb::kI3DL2ReverbDensity, "knobs_small_100");
    density->setPos({ 270, lowerY });
    density->setSize({ 64, 64 });
    density->setStripFrames(DEFAULT_KNOB_FRAMES);
    m_vananagui->insertUIElement(density);


    // Quality buttons
    auto& buttons = m_buttons;
    auto onclickHandler = [this, &buttons](UIButton* self) {
        for (auto b : buttons)
            b->setState(false);
        self->setState(true);

        int i;
        for (i = 0; i < buttons.size(); i++)
            if (buttons[i]->getState()) break;
        this->effect->setParameterAutomated(I3DL2Reverb::kI3DL2ReverbQuality, (float)i / 3.0f);
    };

    auto btnQualityLowMinus = new UIButton(0, "quality0_on_100", "quality0_hover_100");
    auto btnQualityLowPlus = new UIButton(0, "quality1_on_100", "quality1_hover_100");
    auto btnQualityHighMinus = new UIButton(0, "quality2_on_100", "quality2_hover_100");
    auto btnQualityHighPlus = new UIButton(0, "quality3_on_100", "quality3_hover_100");
    btnQualityLowMinus->setPosAndSize({ 182, 277 } , { 38, 24 });
    btnQualityLowPlus->setPosAndSize({ 220, 277 }  , { 38, 24 });
    btnQualityHighMinus->setPosAndSize({ 258, 277 }, { 38, 24 });
    btnQualityHighPlus->setPosAndSize({ 296, 277 } , { 38, 24 });
    btnQualityLowMinus->setClickHandler(onclickHandler);
    btnQualityLowPlus->setClickHandler(onclickHandler);
    btnQualityHighMinus->setClickHandler(onclickHandler);
    btnQualityHighPlus->setClickHandler(onclickHandler);
    btnQualityLowPlus->setState(true); // default

    m_buttons.push_back(btnQualityLowMinus);
    m_buttons.push_back(btnQualityLowPlus);
    m_buttons.push_back(btnQualityHighMinus);
    m_buttons.push_back(btnQualityHighPlus);

    for (auto b : m_buttons) {
        m_vananagui->insertUIElement(b);
    }


    // vananaSun hyperlink
    auto onclickHyperlink = [this](UIButton* self) {
        HINSTANCE hInst = ShellExecute(NULL, NULL, "http://maximvandijk.nl", NULL, NULL, SW_SHOWDEFAULT);
        bool success = (hInst > (HINSTANCE)32);
    };
    auto btnHyperlink = new UIButton("vananasun_off_100", 0, "vananasun_hover_100");
    btnHyperlink->setPosAndSize({28, 279}, {83, 22});
    btnHyperlink->addTexOffset({0, 4});
    btnHyperlink->setClickHandler(onclickHyperlink);
    m_vananagui->insertUIElement(btnHyperlink);


    m_vananagui->render();
    m_vananagui->getWindow().show();
    return true;
}

void RefurbGUI::close() {
    SAFE_DELETE_PTR(m_vananagui);
}

void RefurbGUI::idle() {
    if (m_vananagui->shouldDraw())
        m_vananagui->render();
}

bool RefurbGUI::onWheel(float distance) {
    // @TODO: implement wheel call here!
    // m_vananagui->render();
    return false;
}


void RefurbGUI::updateParameters() {
    // set quality button states
    float quality = this->effect->getParameter(I3DL2Reverb::kI3DL2ReverbQuality);
    for (auto b : m_buttons) {
        b->setState(false);
    }
    m_buttons[(int)(quality * 3.0f)]->setState(true);
}


VananaGUI* RefurbGUI::getVananaGUI() {
    return m_vananagui;
}
