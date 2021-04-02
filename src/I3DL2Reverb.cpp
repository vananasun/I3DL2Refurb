#define _USE_MATH_DEFINES
#include <cmath>
#include "I3DL2Reverb.h"


void I3DL2Reverb::DelayLine::init(int32_t ms, int32_t padding, uint32_t sampleRate, int32_t delayTap) {
	m_length = ((sampleRate * ms) / 1000) + padding;
	m_position = 0;
	setDelayTap(delayTap);
	assign(m_length, 0.0f);
}

void I3DL2Reverb::DelayLine::setDelayTap(int32_t delayTap) {
	if(m_length > 0)
		m_delayPosition = (delayTap + m_position + m_length) % m_length;
}


void I3DL2Reverb::DelayLine::advance() {
	if(--m_position < 0)
		m_position += m_length;
	if(--m_delayPosition < 0)
		m_delayPosition += m_length;
}


__forceinline void I3DL2Reverb::DelayLine::set(float value) {
	at(m_position) = value;
}


float I3DL2Reverb::DelayLine::get(int32_t offset) const {
	offset = (offset + m_position) % m_length;
	if(offset < 0)
		offset += m_length;
	return at(offset);
}

__forceinline float I3DL2Reverb::DelayLine::get() const {
	return at(m_delayPosition);
}



I3DL2Reverb::I3DL2Reverb() {
    m_param[kI3DL2ReverbRoomHF] = 0.99f;
	m_param[kI3DL2ReverbDecayTime] = 0.07f;
	m_param[kI3DL2ReverbDecayHFRatio] = 0.3842105f;
	m_param[kI3DL2ReverbReflections] = 0.672545433f;
	m_param[kI3DL2ReverbReflectionsDelay] = 0.233333333f;
	// m_param[kI3DL2ReverbReverb] = 0.85f;
	m_param[kI3DL2ReverbReverbDelay] = 0.11f;
	m_param[kI3DL2ReverbDiffusion] = 1.0f;
	m_param[kI3DL2ReverbDensity] = 1.0f;
	m_param[kI3DL2ReverbHFReference] = (5000.0f - 20.0f) / 19980.0f;
	m_param[kI3DL2ReverbQuality] = 1.0f / 3.0f; // LQ+
    m_param[kI3DL2ReverbDryWet] = 0.5f;
}


void I3DL2Reverb::process(float** inps, float** outs, uint32_t numFrames) {
    float* inputsL = inps[0];
    float* inputsR = inps[1];
    float* outputsL = outs[0];
    float* outputsR = outs[1];


	if (m_recalcParams) {
		auto sampleRate = m_effectiveSampleRate;
		recalculateI3DL2ReverbParams();
		// Resize and clear delay lines if quality has changed
		if(sampleRate != m_effectiveSampleRate)
			positionChanged();
	}

    uint32_t frames = numFrames;
	if(!(m_quality & kFullSampleRate) && m_remain && frames > 0) {
		// Remaining frame from previous render call
		frames--;
		*(outputsL++) = m_prevL;
		*(outputsR++) = m_prevR;
		inputsL++;
		inputsR++;
		m_remain = false;
	}

    while (frames > 0) {
  		// Apply room filter and insert into early reflection delay lines
  		const float inL = *(inputsL++);
  		const float inRoomL = (m_filterHist[12] - inL) * m_roomFilter + inL;
  		m_filterHist[12] = inRoomL;
  		m_delayLines[15].set(inRoomL);

  		const float inR = *(inputsR++);
  		const float inRoomR = (m_filterHist[13] - inR) * m_roomFilter + inR;
  		m_filterHist[13] = inRoomR;
  		m_delayLines[16].set(inRoomR);

  		// Early reflections (left)
  		float earlyL = m_delayLines[15].get(m_earlyTaps[0][1]) * 0.68f
  			- m_delayLines[15].get(m_earlyTaps[0][2]) * 0.5f
  			- m_delayLines[15].get(m_earlyTaps[0][3]) * 0.62f
  			- m_delayLines[15].get(m_earlyTaps[0][4]) * 0.5f
  			- m_delayLines[15].get(m_earlyTaps[0][5]) * 0.62f;
  		if(m_quality & kMoreDelayLines)
  		{
  			float earlyL2 = earlyL;
  			earlyL = m_delayLines[13].get() + earlyL * 0.618034f;
  			m_delayLines[13].set(earlyL2 - earlyL * 0.618034f);
  		}
  		const float earlyRefOutL = earlyL * m_ERLevel;
  		m_filterHist[15] = m_delayLines[15].get(m_earlyTaps[0][0]) + m_filterHist[15];
  		m_filterHist[16] = m_delayLines[16].get(m_earlyTaps[1][0]) + m_filterHist[16];

  		// Lots of slightly different copy-pasta ahead
  		float reverbL1, reverbL2, reverbL3, reverbR1, reverbR2, reverbR3;

  		reverbL1 = -m_filterHist[15] * 0.707f;
  		reverbL2 = m_filterHist[16] * 0.707f + reverbL1;
  		reverbR2 = reverbL1 - m_filterHist[16] * 0.707f;

  		m_filterHist[5] = (m_filterHist[5] - m_delayLines[5].get()) * m_delayCoeffs[5][1] + m_delayLines[5].get();
  		reverbL1 = m_filterHist[5] * m_delayCoeffs[5][0] + reverbL2 * m_diffusion;
  		m_delayLines[5].set(reverbL2 - reverbL1 * m_diffusion);
  		reverbL2 = reverbL1;
  		reverbL3 = -0.15f * reverbL1;

  		m_filterHist[4] = (m_filterHist[4] - m_delayLines[4].get()) * m_delayCoeffs[4][1] + m_delayLines[4].get();
  		reverbL1 = m_filterHist[4] * m_delayCoeffs[4][0] + reverbL2 * m_diffusion;
  		m_delayLines[4].set(reverbL2 - reverbL1 * m_diffusion);
  		reverbL2 = reverbL1;
  		reverbL3 -= reverbL1 * 0.2f;

  		if(m_quality & kMoreDelayLines)
  		{
  			m_filterHist[3] = (m_filterHist[3] - m_delayLines[3].get()) * m_delayCoeffs[3][1] + m_delayLines[3].get();
  			reverbL1 = m_filterHist[3] * m_delayCoeffs[3][0] + reverbL2 * m_diffusion;
  			m_delayLines[3].set(reverbL2 - reverbL1 * m_diffusion);
  			reverbL2 = reverbL1;
  			reverbL3 += 0.35f * reverbL1;

  			m_filterHist[2] = (m_filterHist[2] - m_delayLines[2].get()) * m_delayCoeffs[2][1] + m_delayLines[2].get();
  			reverbL1 = m_filterHist[2] * m_delayCoeffs[2][0] + reverbL2 * m_diffusion;
  			m_delayLines[2].set(reverbL2 - reverbL1 * m_diffusion);
  			reverbL2 = reverbL1;
  			reverbL3 -= reverbL1 * 0.38f;
  		}
  		m_delayLines[17].set(reverbL2);

  		reverbL1 = m_delayLines[17].get() * m_delayCoeffs[12][0];
  		m_filterHist[17] = (m_filterHist[17] - reverbL1) * m_delayCoeffs[12][1] + reverbL1;

  		m_filterHist[1] = (m_filterHist[1] - m_delayLines[1].get()) * m_delayCoeffs[1][1] + m_delayLines[1].get();
  		reverbL1 = m_filterHist[17] * m_diffusion + m_filterHist[1] * m_delayCoeffs[1][0];
  		m_delayLines[1].set(m_filterHist[17] - reverbL1 * m_diffusion);
  		reverbL2 = reverbL1;
  		float reverbL4 = reverbL1 * 0.38f;

  		m_filterHist[0] = (m_filterHist[0] - m_delayLines[0].get()) * m_delayCoeffs[0][1] + m_delayLines[0].get();
  		reverbL1 = m_filterHist[0] * m_delayCoeffs[0][0] + reverbL2 * m_diffusion;
  		m_delayLines[0].set(reverbL2 - reverbL1 * m_diffusion);
  		reverbL3 -= reverbL1 * 0.38f;
  		m_filterHist[15] = reverbL1;

  		// Early reflections (right)
  		float earlyR = m_delayLines[16].get(m_earlyTaps[1][1]) * 0.707f
  			- m_delayLines[16].get(m_earlyTaps[1][2]) * 0.6f
  			- m_delayLines[16].get(m_earlyTaps[1][3]) * 0.5f
  			- m_delayLines[16].get(m_earlyTaps[1][4]) * 0.6f
  			- m_delayLines[16].get(m_earlyTaps[1][5]) * 0.5f;
  		if(m_quality & kMoreDelayLines)
  		{
  			float earlyR2 = earlyR;
  			earlyR = m_delayLines[14].get() + earlyR * 0.618034f;
  			m_delayLines[14].set(earlyR2 - earlyR * 0.618034f);
  		}
  		const float earlyRefOutR = earlyR * m_ERLevel;

  		m_filterHist[11] = (m_filterHist[11] - m_delayLines[11].get()) * m_delayCoeffs[11][1] + m_delayLines[11].get();
  		reverbR1 = m_filterHist[11] * m_delayCoeffs[11][0] + reverbR2 * m_diffusion;
  		m_delayLines[11].set(reverbR2 - reverbR1 * m_diffusion);
  		reverbR2 = reverbR1;

  		m_filterHist[10] = (m_filterHist[10] - m_delayLines[10].get()) * m_delayCoeffs[10][1] + m_delayLines[10].get();
  		reverbR1 = m_filterHist[10] * m_delayCoeffs[10][0] + reverbR2 * m_diffusion;
  		m_delayLines[10].set(reverbR2 - reverbR1 * m_diffusion);
  		reverbR3 = reverbL4 - reverbR2 * 0.15f - reverbR1 * 0.2f;
  		reverbR2 = reverbR1;

  		if(m_quality & kMoreDelayLines)
  		{
  			m_filterHist[9] = (m_filterHist[9] - m_delayLines[9].get()) * m_delayCoeffs[9][1] + m_delayLines[9].get();
  			reverbR1 = m_filterHist[9] * m_delayCoeffs[9][0] + reverbR2 * m_diffusion;
  			m_delayLines[9].set(reverbR2 - reverbR1 * m_diffusion);
  			reverbR2 = reverbR1;
  			reverbR3 += reverbR1 * 0.35f;

  			m_filterHist[8] = (m_filterHist[8] - m_delayLines[8].get()) * m_delayCoeffs[8][1] + m_delayLines[8].get();
  			reverbR1 = m_filterHist[8] * m_delayCoeffs[8][0] + reverbR2 * m_diffusion;
  			m_delayLines[8].set(reverbR2 - reverbR1 * m_diffusion);
  			reverbR2 = reverbR1;
  			reverbR3 -= reverbR1 * 0.38f;
  		}
  		m_delayLines[18].set(reverbR2);

  		reverbR1 = m_delayLines[18].get() * m_delayCoeffs[12][0];
  		m_filterHist[18] = (m_filterHist[18] - reverbR1) * m_delayCoeffs[12][1] + reverbR1;

  		m_filterHist[7] = (m_filterHist[7] - m_delayLines[7].get()) * m_delayCoeffs[7][1] + m_delayLines[7].get();
  		reverbR1 = m_filterHist[18] * m_diffusion + m_filterHist[7] * m_delayCoeffs[7][0];
  		m_delayLines[7].set(m_filterHist[18] - reverbR1 * m_diffusion);
  		reverbR2 = reverbR1;

  		float lateRevOutL = (reverbL3 + reverbR1 * 0.38f) * m_ReverbLevelL;

  		m_filterHist[6] = (m_filterHist[6] - m_delayLines[6].get()) * m_delayCoeffs[6][1] + m_delayLines[6].get();
  		reverbR1 = m_filterHist[6] * m_delayCoeffs[6][0] + reverbR2 * m_diffusion;
  		m_delayLines[6].set(reverbR2 - reverbR1 * m_diffusion);
  		m_filterHist[16] = reverbR1;

  		float lateRevOutR = (reverbR3 - reverbR1 * 0.38f) * m_ReverbLevelR;

  		float outL = earlyRefOutL + lateRevOutL;
  		float outR = earlyRefOutR + lateRevOutR;

  		for(auto &line : m_delayLines)
			   line.advance();


        float dryMultiplier = std::min(1.0, 2.0 - m_param[kI3DL2ReverbDryWet] * 2.0);

  		if(!(m_quality & kFullSampleRate)) { // LQ- and LQ+
            *(outputsL++) = inL * dryMultiplier + ((outL + m_prevL) * 0.5f * m_param[kI3DL2ReverbDryWet]);
            *(outputsR++) = inR * dryMultiplier + ((outR + m_prevR) * 0.5f * m_param[kI3DL2ReverbDryWet]);
            m_prevL = outL;
            m_prevR = outR;
            inputsL++;
            inputsR++;
            if (frames-- == 1) {
                m_remain = true;
                break;
            }
  		}

  		*(outputsL++) = inL * dryMultiplier + outL * m_param[kI3DL2ReverbDryWet];
  		*(outputsR++) = inR * dryMultiplier + outR * m_param[kI3DL2ReverbDryWet];
  		frames--;
    }

}





void I3DL2Reverb::setParameter(uint32_t index, float value) {
    if (index < kI3DL2ReverbNumParameters) {
        // limit between 0 and 1
        if (value < 0.0f) value = 0.0f;
        else if (value > 1.0f) value = 1.0f;

		if (index == kI3DL2ReverbQuality)
			value = std::round(value * 3.0f) / 3.0f;
		m_param[index] = value;
		m_recalcParams = true;
	}
}

float I3DL2Reverb::getParameter(uint32_t index) {
    if (index < kI3DL2ReverbNumParameters)
        return m_param[index];
    return 0;
}



void I3DL2Reverb::positionChanged() {
    // memset zero
    for (auto i = 0; i < 19; i++) m_filterHist[i] = 0.0f;

	m_prevL = 0;
	m_prevR = 0;
	m_remain = false;

	// try {
	uint32_t sampleRate = static_cast<uint32_t>(m_effectiveSampleRate);
	m_delayLines[0].init(67, 5, sampleRate, m_delayTaps[0]);
	m_delayLines[1].init(62, 5, sampleRate, m_delayTaps[1]);
	m_delayLines[2].init(53, 5, sampleRate, m_delayTaps[2]);
	m_delayLines[3].init(43, 5, sampleRate, m_delayTaps[3]);
	m_delayLines[4].init(32, 5, sampleRate, m_delayTaps[4]);
	m_delayLines[5].init(22, 5, sampleRate, m_delayTaps[5]);
	m_delayLines[6].init(75, 5, sampleRate, m_delayTaps[6]);
	m_delayLines[7].init(69, 5, sampleRate, m_delayTaps[7]);
	m_delayLines[8].init(60, 5, sampleRate, m_delayTaps[8]);
	m_delayLines[9].init(48, 5, sampleRate, m_delayTaps[9]);
	m_delayLines[10].init(36, 5, sampleRate, m_delayTaps[10]);
	m_delayLines[11].init(25, 5, sampleRate, m_delayTaps[11]);
	m_delayLines[12].init(0, 0, 0);	// Dummy for array index consistency with both tap and coefficient arrays
	m_delayLines[13].init(3, 0, sampleRate, m_delayTaps[13]);
	m_delayLines[14].init(3, 0, sampleRate, m_delayTaps[14]);
	m_delayLines[15].init(407, 1, sampleRate);
	m_delayLines[16].init(400, 1, sampleRate);
	m_delayLines[17].init(10, 0, sampleRate, -1);
	m_delayLines[18].init(10, 0, sampleRate, -1);
}


template<class T, class C>
inline T Clamp(T val, const C lowerLimit, const C upperLimit) {
	if(val < lowerLimit) return lowerLimit;
	else if(val > upperLimit) return upperLimit;
	else return val;
}

void I3DL2Reverb::recalculateI3DL2ReverbParams() {
	m_quality = quality();
	m_effectiveSampleRate = m_sampleRate / ((m_quality & kFullSampleRate) ? 1u : 2u);
    //static_cast<float>(m_SndFile.getSampleRate() / ((m_quality & kFullSampleRate) ? 1u : 2u));

	// Diffusion
	m_diffusion = diffusion() * (0.618034f / 100.0f);
	// Early Reflection Level
	m_ERLevel = std::min(std::pow(10.0f, reflections() / (100.0f * 20.0f)), 1.0f) * 0.761f;

	// Room Filter
	float rmHF = std::pow(10.0f, roomHF() / 100.0f / 10.0f);
	if(rmHF == 1.0f) {
		m_roomFilter = 0.0f;
	} else {
		float freq = std::cos(hfReference() * static_cast<float>(2.0 * M_PI) / m_effectiveSampleRate);
		float roomFilter = (freq * (rmHF + rmHF) - 2.0f + std::sqrt(freq * (rmHF * rmHF * freq * 4.0f) + rmHF * 8.0f - rmHF * rmHF * 4.0f - rmHF * freq * 8.0f)) / (rmHF + rmHF - 2.0f);
		m_roomFilter = Clamp(roomFilter, 0.0f, 1.0f);
	}

	setDelayTaps();
	setDecayCoeffs();

	m_recalcParams = false;
}


void I3DL2Reverb::setDelayTaps() {
	// Early reflections
	static constexpr float delays[] =
	{
		1.0000f, 1.0000f, 0.0000f, 0.1078f, 0.1768f, 0.2727f,
		0.3953f, 0.5386f, 0.6899f, 0.8306f, 0.9400f, 0.9800f,
	};

	const float sampleRate = m_effectiveSampleRate;
	const float reflectionsDelay_ = reflectionsDelay();
	const float reverbDelay_ = std::max(reverbDelay(), 5.0f / 1000.0f);
	m_earlyTaps[0][0] = static_cast<int32_t>((reverbDelay_ + reflectionsDelay_ + 7.0f / 1000.0f) * sampleRate);
	for (uint32_t i = 1; i < 12; i++) {
		m_earlyTaps[i % 2u][i / 2u] = static_cast<int32_t>((reverbDelay_ * delays[i] + reflectionsDelay_) * sampleRate);
	}

	// Late reflections
	float density_ = std::min((density() / 100.0f + 0.1f) * 0.9091f, 1.0f);
	float delayL = density_ * 67.0f / 1000.0f * sampleRate;
	float delayR = density_ * 75.0f / 1000.0f * sampleRate;
	for(int i = 0, power = 0; i < 6; i++) {
		power += i;
		float factor = std::pow(0.93f, static_cast<float>(power));
		m_delayTaps[i + 0] = static_cast<int32_t>(delayL * factor);
		m_delayTaps[i + 6] = static_cast<int32_t>(delayR * factor);
	}
	m_delayTaps[12] = static_cast<int32_t>(10.0f / 1000.0f * sampleRate);
	// Early reflections (extra delay lines)
	m_delayTaps[13] = static_cast<int32_t>(3.25f / 1000.0f * sampleRate);
	m_delayTaps[14] = static_cast<int32_t>(3.53f / 1000.0f * sampleRate);

	for(std::size_t d = 0; d < std::size(m_delayTaps); d++)
		m_delayLines[d].setDelayTap(m_delayTaps[d]);
}


void I3DL2Reverb::setDecayCoeffs()
{
	float levelLtmp = 1.0f, levelRtmp = 1.0f;
	float levelL = 0.0f, levelR = 0.0f;

	levelLtmp *= calcDecayCoeffs(5);
	levelRtmp *= calcDecayCoeffs(11);
	levelL += levelLtmp * 0.0225f;
	levelR += levelRtmp * 0.0225f;

	levelLtmp *= calcDecayCoeffs(4);
	levelRtmp *= calcDecayCoeffs(10);
	levelL += levelLtmp * 0.04f;
	levelR += levelRtmp * 0.04f;

	if(m_quality & kMoreDelayLines) {
		levelLtmp *= calcDecayCoeffs(3);
		levelRtmp *= calcDecayCoeffs(9);
		levelL += levelLtmp * 0.1225f;
		levelR += levelRtmp * 0.1225f;

		levelLtmp *= calcDecayCoeffs(2);
		levelRtmp *= calcDecayCoeffs(8);
		levelL += levelLtmp * 0.1444f;
		levelR += levelRtmp * 0.1444f;
	}
	calcDecayCoeffs(12);
	levelLtmp *= m_delayCoeffs[12][0] * m_delayCoeffs[12][0];
	levelRtmp *= m_delayCoeffs[12][0] * m_delayCoeffs[12][0];

	levelLtmp *= calcDecayCoeffs(1);
	levelRtmp *= calcDecayCoeffs(7);
	levelL += levelRtmp * 0.1444f;
	levelR += levelLtmp * 0.1444f;

	levelLtmp *= calcDecayCoeffs(0);
	levelRtmp *= calcDecayCoeffs(6);
	levelL += levelLtmp * 0.1444f;
	levelR += levelRtmp * 0.1444f;

	// Final Reverb Level
	float level = std::min(std::pow(10.0f, reverb() / (100.0f * 20.0f)), 1.0f);
	float monoInv = 1.0f - ((levelLtmp + levelRtmp) * 0.5f);
	m_ReverbLevelL = level * std::sqrt(monoInv / levelL);
	m_ReverbLevelR = level * std::sqrt(monoInv / levelR);
}


float I3DL2Reverb::calcDecayCoeffs(int32_t index) {
	float hfRef = static_cast<float>(2.0 * M_PI) / m_effectiveSampleRate * hfReference();
	float decayHFRatio_ = decayHFRatio();
	if(decayHFRatio_ > 1.0f)
		hfRef = static_cast<float>(M_PI);

	float c1 = std::pow(10.0f, ((m_delayTaps[index] / m_effectiveSampleRate) * -60.0f / decayTime()) / 20.0f);
	float c2 = 0.0f;

	float c21 = (std::pow(c1, 2.0f - 2.0f / decayHFRatio_) - 1.0f) / (1.0f - std::cos(hfRef));
	if(c21 != 0)
	{
		float c22 = -2.0f * c21 - 2.0f;
		float c23 = std::sqrt(c22 * c22 - c21 * c21 * 4.0f);
		c2 = (c23 - c22) / (c21 + c21);
		if(std::abs(c2) > 1.0f)
			c2 = (-c22 - c23) / (c21 + c21);
	}
	m_delayCoeffs[index][0] = c1;
	m_delayCoeffs[index][1] = c2;

	c1 *= c1;
	float diff2 = m_diffusion * m_diffusion;
	return diff2 + c1 / (1.0f - diff2 * c1) * (1.0f - diff2) * (1.0f - diff2);
}
