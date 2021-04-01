#pragma once
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include <vananagui/VananaGUI.h>
#include <vector>


class RefurbGUI : public AEffEditor {
private:
    ERect rect;
    AudioEffect* effect;
    VananaGUI* m_vananagui;
    std::vector<UIButton*> m_buttons;
public:
    RefurbGUI(AudioEffect* effect);
    ~RefurbGUI();

    bool getRect(ERect** rect);
    bool isOpen();
    bool open(void* ptr);
    void close();
    void idle();

    bool onWheel(float distance);

    void updateParameters();
    AudioEffect* getEffect() { return this->effect; }
    VananaGUI* getVananaGUI();
};
