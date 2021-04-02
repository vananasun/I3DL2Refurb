#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "Refurb.h"
#include "RefurbGUI.h"
#include "I3DL2Reverb.h"
#ifdef DEBUG
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#endif


AudioEffect *createEffectInstance(audioMasterCallback audioMaster) {
    return new Refurb(audioMaster);
}

Refurb::Refurb(audioMasterCallback audioMaster) : AudioEffectX(audioMaster, 1, I3DL2Reverb::kI3DL2ReverbNumParameters) {
    #ifdef DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    #endif

    this->reverb = new I3DL2Reverb();
    this->reverb->setSampleRate(updateSampleRate());
    this->editor = new RefurbGUI(this);
    setEditor(this->editor);

    setNumInputs(2);
    setNumOutputs(2);
    DECLARE_VST_DEPRECATED(canMono)();
    setUniqueID('vSR0');
    canProcessReplacing();
}

bool Refurb::getProductString(char* text) { strcpy(text, "I3DL2 Refurb 1.0"); return true; }
bool Refurb::getVendorString(char* text)  { strcpy(text, "vananaSun"); return true; }
bool Refurb::getEffectName(char* name)    { strcpy(name, "I3DL2Refurb"); return true; }

void Refurb::suspend() {}

void Refurb::setSampleRate(float sampleRate) {
    this->reverb->setSampleRate(sampleRate);
}


Refurb::~Refurb() {
    delete this->reverb;
	// if(programs) delete [] programs;
}


void Refurb::setProgramName(char *name) { /* strcpy(programs[curProgram].name, name); */ }
void Refurb::getProgramName(char *name) { /* strcpy(name, programs[curProgram].name); */ }
bool Refurb::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name) {
	// if ((unsigned int)index < NPROGS)
	// {
	//     strcpy(name, programs[index].name);
	//     return true;
	// }
	return false;
}


void Refurb::setProgram(VstInt32 program) {
	// curProgram = program;
	// gain = (float)pow(10.0f, 2.0f * programs[curProgram].param[_PARAM0] - 1.0f);
	//if(editor) editor->postUpdate();
}

float Refurb::getParameter(VstInt32 index) {
    return this->reverb->getParameter(index);
}

void Refurb::setParameter(VstInt32 index, float value) {
    this->reverb->setParameter(index, value);
    this->editor->updateParameters();
    this->editor->getVananaGUI()->redraw();
    // this->editor->getVananaGUI()->render(); // @TODO: get rid of double render call
}

void Refurb::setParameterAutomated(VstInt32 index, float value) {
    audioMaster(&cEffect, audioMasterAutomate, index, 0, nullptr, value);
    setParameter(index, value);
    this->editor->getVananaGUI()->render(); // @TODO: test if this can be rid of
}

void Refurb::getParameterName(VstInt32 index, char* label) {
    switch(index) {
    case I3DL2Reverb::kI3DL2ReverbRoomHF: strcpy(label, "RoomHF"); break;
    case I3DL2Reverb::kI3DL2ReverbDecayTime: strcpy(label, "DecayTime"); break;
    case I3DL2Reverb::kI3DL2ReverbDecayHFRatio: strcpy(label, "DecayHFRatio"); break;
    case I3DL2Reverb::kI3DL2ReverbReflections: strcpy(label, "Reflections"); break;
    case I3DL2Reverb::kI3DL2ReverbReflectionsDelay: strcpy(label, "ReflectionsDelay"); break;
    // case I3DL2Reverb::kI3DL2ReverbReverb: strcpy(label, "Reverb"); break;
    case I3DL2Reverb::kI3DL2ReverbReverbDelay: strcpy(label, "ReverbDelay"); break;
    case I3DL2Reverb::kI3DL2ReverbDiffusion: strcpy(label, "Diffusion"); break;
    case I3DL2Reverb::kI3DL2ReverbDensity: strcpy(label, "Density"); break;
    case I3DL2Reverb::kI3DL2ReverbHFReference: strcpy(label, "HFReference"); break;
    case I3DL2Reverb::kI3DL2ReverbQuality: strcpy(label, "Quality"); break;
    case I3DL2Reverb::kI3DL2ReverbDryWet: strcpy(label, "Dry/Wet"); break;
    default: strcpy(label,"");
    }
}

void Refurb::getParameterDisplay(VstInt32 index, char* text) {
    static constexpr const char* const modes[] = { "LQ", "LQ+", "HQ", "HQ+" };
    float value = 0.0f;
    switch(index) {
    case I3DL2Reverb::kI3DL2ReverbRoomHF: value = this->reverb->roomHF() * 0.01f; break;
    case I3DL2Reverb::kI3DL2ReverbDecayTime: value = this->reverb->decayTime(); break;
    case I3DL2Reverb::kI3DL2ReverbDecayHFRatio: value = this->reverb->decayHFRatio(); break;
    case I3DL2Reverb::kI3DL2ReverbReflections: value = this->reverb->reflections() * 0.01f; break;
    case I3DL2Reverb::kI3DL2ReverbReflectionsDelay: value = this->reverb->reflectionsDelay(); break;
    // case I3DL2Reverb::kI3DL2ReverbReverb: value = this->reverb->reverb() * 0.01f; break;
    case I3DL2Reverb::kI3DL2ReverbReverbDelay: value = this->reverb->reverbDelay(); break;
    case I3DL2Reverb::kI3DL2ReverbDiffusion: value = this->reverb->diffusion(); break;
    case I3DL2Reverb::kI3DL2ReverbDensity: value = this->reverb->density(); break;
    case I3DL2Reverb::kI3DL2ReverbHFReference: value = this->reverb->hfReference(); break;
    case I3DL2Reverb::kI3DL2ReverbQuality: strcpy(text, modes[this->reverb->quality() % 4u]); return;
    case I3DL2Reverb::kI3DL2ReverbDryWet: value = this->reverb->dryWet(); break;
    }

    char string[16] = { 0 };
    sprintf(string, "%.2f", value);
    strcpy(text, (char*)string);
}


void Refurb::getParameterLabel(VstInt32 index, char* label) {
    switch(index) {
    case I3DL2Reverb::kI3DL2ReverbRoomHF:
    case I3DL2Reverb::kI3DL2ReverbReflections:
    // case I3DL2Reverb::kI3DL2ReverbReverb:
        strcpy(label, "dB");
    case I3DL2Reverb::kI3DL2ReverbDecayTime:
    case I3DL2Reverb::kI3DL2ReverbReflectionsDelay:
    case I3DL2Reverb::kI3DL2ReverbReverbDelay:
        strcpy(label, "s");
    case I3DL2Reverb::kI3DL2ReverbDiffusion:
    case I3DL2Reverb::kI3DL2ReverbDensity:
    case I3DL2Reverb::kI3DL2ReverbDryWet:
        strcpy(label, "%");
    case I3DL2Reverb::kI3DL2ReverbHFReference:
        strcpy(label, "Hz");
    default: strcpy(label, "");
    }
}




void Refurb::process(float** inputs, float** outputs, VstInt32 sampleFrames) {
    this->processReplacing(inputs, outputs, sampleFrames);
}

void Refurb::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
    this->reverb->process(inputs, outputs, sampleFrames);
}
