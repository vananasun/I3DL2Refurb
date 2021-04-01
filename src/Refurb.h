#include <public.sdk/source/vst2.x/audioeffectx.h>
#include "I3DL2Reverb.h"
#include "RefurbGUI.h"

class Refurb : public AudioEffectX {
private:
    I3DL2Reverb* reverb;
    RefurbGUI* editor;
public:
    Refurb(audioMasterCallback audioMaster);
    ~Refurb();

	virtual void process(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
    virtual void  setProgram(VstInt32 program);
    virtual void  setProgramName(char *name);
    virtual void  getProgramName(char *name);
    virtual bool  getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name);
    virtual void  setParameter(VstInt32 index, float value);
    virtual void  setParameterAutomated(VstInt32 index, float value);
    virtual float getParameter(VstInt32 index);
    virtual void  getParameterLabel(VstInt32 index, char *label);
    virtual void  getParameterDisplay(VstInt32 index, char *text);
    virtual void  getParameterName(VstInt32 index, char *text);
    virtual void  setSampleRate(float sampleRate);
    virtual void  suspend();

    virtual bool getEffectName(char *name);
    virtual bool getVendorString(char *text);
    virtual bool getProductString(char *text);
    virtual VstInt32 getVendorVersion() { return 1000; }
};
