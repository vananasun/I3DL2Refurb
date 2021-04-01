#pragma once
#include <public.sdk/source/vst2.x/audioeffectx.h>
#include <stdint.h>
#include <vector>
#include <cmath>

class I3DL2Reverb {
public:
	enum Parameters {
		kI3DL2ReverbRoomHF = 0,
		kI3DL2ReverbDecayTime,
		kI3DL2ReverbDecayHFRatio,
		kI3DL2ReverbReflections,
		kI3DL2ReverbReflectionsDelay,
		kI3DL2ReverbReverb,
		kI3DL2ReverbReverbDelay,
		kI3DL2ReverbDiffusion,
		kI3DL2ReverbDensity,
		kI3DL2ReverbHFReference,
		kI3DL2ReverbQuality,
        kI3DL2ReverbDryWet,
		kI3DL2ReverbNumParameters
	};
private:

    enum QualityFlags {
        kMoreDelayLines = 0x01,
        kFullSampleRate = 0x02
    };


    class DelayLine : private std::vector<float> {
        int32_t m_length;
        int32_t m_position;
        int32_t m_delayPosition;
    public:
        void init(int32_t ms, int32_t padding, uint32_t sampleRate, int32_t delayTap = 0);
        void setDelayTap(int32_t delayTap);
        void advance();
        void set(float value);
        float get(int32_t offset) const;
        float get() const;
    };


    float m_param[kI3DL2ReverbNumParameters];
    float m_sampleRate = 44100.0f;

	// Calculated parameters
	uint32_t m_quality;
	float m_effectiveSampleRate;
	float m_diffusion;
	float m_roomFilter;
	float m_ERLevel;
	float m_ReverbLevelL;
	float m_ReverbLevelR;

    int32_t m_delayTaps[15];	// 6*L + 6*R + LR + Early L + Early R
    int32_t m_earlyTaps[2][6];
    float m_delayCoeffs[13][2];

    // State
    DelayLine m_delayLines[19];
    float m_filterHist[19];

    // Remaining frame for downsampled reverb
    float m_prevL;
    float m_prevR;
    bool m_remain = false;

    bool m_recalcParams = true;


public:
    I3DL2Reverb();
    void setSampleRate(float rate) { m_sampleRate = rate; };
    void process(float** inputs, float** outputs, uint32_t numFrames);
	void positionChanged();
    void recalculateI3DL2ReverbParams();
	void setDelayTaps();
	void setDecayCoeffs();
	float calcDecayCoeffs(int32_t index);

    void setParameter(uint32_t index, float value);
    float getParameter(uint32_t index);
 
	float roomHF() const { return -10000.0f + m_param[kI3DL2ReverbRoomHF] * 10000.0f; }
	float decayTime() const { return 0.1f + m_param[kI3DL2ReverbDecayTime] * 19.9f; }
	float decayHFRatio() const { return 0.1f + m_param[kI3DL2ReverbDecayHFRatio] * 1.9f; }
	float reflections() const { return -10000.0f + m_param[kI3DL2ReverbReflections] * 11000.0f; }
	float reflectionsDelay() const { return m_param[kI3DL2ReverbReflectionsDelay] * 0.3f; }
	float reverb() const { return -10000.0f + m_param[kI3DL2ReverbReverb] * 12000.0f; }
	float reverbDelay() const { return m_param[kI3DL2ReverbReverbDelay] * 0.1f; }
	float diffusion() const { return m_param[kI3DL2ReverbDiffusion] * 100.0f; }
	float density() const { return m_param[kI3DL2ReverbDensity] * 100.0f; }
	float hfReference() const { return 20.0f + m_param[kI3DL2ReverbHFReference] * 19980.0f; }
	uint32_t quality() const { return (uint32_t)std::round(m_param[kI3DL2ReverbQuality] * 3.0f); }
    float dryWet() const { return m_param[kI3DL2ReverbDryWet] * 100.0f; }
};


//
// public:
// 	static IMixPlugin* Create(VSTPluginLib &factory, CSoundFile &sndFile, SNDMIXPLUGIN *mixStruct);
// 	I3DL2Reverb(VSTPluginLib &factory, CSoundFile &sndFile, SNDMIXPLUGIN *mixStruct);
//
// 	void Release() override { delete this; }
// 	int32 GetUID() const override { return 0xEF985E71; }
// 	int32 GetVersion() const override { return 0; }
// 	void Idle() override { }
// 	uint32 GetLatency() const override { return 0; }
//
// 	void Process(float *pOutL, float *pOutR, uint32 numFrames) override;
//
// 	float RenderSilence(uint32) override { return 0.0f; }
//
// 	int32 GetNumPrograms() const override;
// 	int32 GetCurrentProgram() override { return m_program; }
// 	// cppcheck-suppress virtualCallInConstructor
// 	void SetCurrentProgram(int32) override;
//
// 	PlugParamIndex GetNumParameters() const override { return kI3DL2ReverbNumParameters; }
// 	PlugParamValue GetParameter(PlugParamIndex index) override;
// 	void SetParameter(PlugParamIndex index, PlugParamValue value) override;
//
// 	void Resume() override;
// 	void Suspend() override { m_isResumed = false; }
// 	void PositionChanged() override;
// 	bool IsInstrument() const override { return false; }
// 	bool CanRecieveMidiEvents() override { return false; }
// 	bool ShouldProcessSilence() override { return true; }
//

//
// 	void BeginSetProgram(int32) override { }
// 	void EndSetProgram() override { }
//
// 	int GetNumInputChannels() const override { return 2; }
// 	int GetNumOutputChannels() const override { return 2; }
//

//
// 	void RecalculateI3DL2ReverbParams();
//
// 	void SetDelayTaps();
// 	void SetDecayCoeffs();
// 	float CalcDecayCoeffs(int32 index);
// };
